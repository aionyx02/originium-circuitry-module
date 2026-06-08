#include "Solver.h"

#include <algorithm>
#include <numeric>

#include "WinChecker.h"

namespace {

// Number of occupied cells in a part's shape (rotation-independent).
int cellCount(const Part& p) {
    int n = 0;
    for (const auto& rowVec : p.shape)
        for (bool b : rowVec) if (b) ++n;
    return n;
}

// The distinct rotations of a part: a square or symmetric piece may look the
// same at several angles, so we skip duplicate orientations to cut branching.
std::vector<Rotate> distinctRotations(const Part& p) {
    std::vector<Rotate> out;
    std::vector<std::vector<std::vector<bool>>> seen;
    for (Rotate r : {Rotate::CW_0, Rotate::CW_90, Rotate::CW_180, Rotate::CW_270}) {
        Part tmp = p;
        tmp.location.rotate = r;
        auto shape = tmp.rotatedShape();
        if (std::find(seen.begin(), seen.end(), shape) == seen.end()) {
            seen.push_back(std::move(shape));
            out.push_back(r);
        }
    }
    return out;
}

// Number of occupied cells in a part's shape (rotation-independent), reused by
// both the ordering heuristic and the balance pre-check below.
//
// Opening feasibility check (O(cells), runs once before any search). Every part
// MUST be placed to win, so on any finished board the total cells of a given
// color are fixed: (that color's fixed cells) + (sum of that color's part
// sizes). The win condition also forces each line's count to match its
// requirement, so that total must equal BOTH the row-constraint sum and the
// col-constraint sum for the color. If any color violates this, no placement
// can ever win — reject immediately instead of exhausting the whole tree.
bool passesBalanceCheck(const Board& board, const std::vector<Part>& parts) {
    const int C = static_cast<int>(board.colors);
    const int M = static_cast<int>(board.rows);
    const int N = static_cast<int>(board.cols);

    std::vector<long> supply(C, 0);
    for (const auto& p : parts) {
        const int color = static_cast<int>(p.colorIndex);
        if (color >= 0 && color < C) supply[color] += cellCount(p);
    }
    for (int r = 0; r < M; ++r) {
        for (int c = 0; c < N; ++c) {
            const int cell = board._boardInfo[r][c];
            if (Board::isCannotMove(cell)) {
                const int color = Board::cannotMoveColor(cell);
                if (color >= 0 && color < C) ++supply[color];
            }
        }
    }

    for (int color = 0; color < C; ++color) {
        long rowSum = 0, colSum = 0;
        for (int r = 0; r < M; ++r) rowSum += board._constraints[color][r];
        for (int c = 0; c < N; ++c) colSum += board._constraints[color][M + c];
        if (rowSum != colSum || rowSum != supply[color]) return false;
    }
    return true;
}

struct Search {
    Board board;
    std::vector<Part> parts;
    std::vector<int> order;                 // part vector-indices, biggest first
    std::vector<std::vector<Rotate>> rots;  // distinct rotations per part index
    std::vector<PartLocation> result;       // aligned to parts by index
    long budget;

    // Incremental state, maintained as parts are placed/removed, so each node
    // pays only O(part cells · colors) instead of an O(colors·(M+N)·edge)
    // whole-board rescan:
    //   rowCnt[color][r] / colCnt[color][c] = cells of that color on that line
    //   rowEmpty[r] / colEmpty[c]           = still-EMPTY cells on that line
    std::vector<std::vector<int>> rowCnt;   // [color][row]
    std::vector<std::vector<int>> colCnt;   // [color][col]
    std::vector<int> rowEmpty;              // [row]
    std::vector<int> colEmpty;              // [col]

    // Precomputed cell offsets for every (part, distinct rotation). A part's
    // rotated cells depend only on its shape + rotation, never on where it sits —
    // yet the old code re-rotated and re-allocated them on every canPlace of
    // every board position (hundreds of heap allocations per node). cellsByRot
    // computes each once; extR/extC are the rotated bbox extents used to trim the
    // placement scan so every tested position is in-bounds without a check.
    std::vector<std::vector<std::vector<std::pair<int, int>>>> cellsByRot;  // [pos][rotIdx]
    std::vector<std::vector<int>> extR;     // [pos][rotIdx] max row offset
    std::vector<std::vector<int>> extC;     // [pos][rotIdx] max col offset

    void initCells() {
        const std::size_t n = parts.size();
        cellsByRot.resize(n);
        extR.resize(n);
        extC.resize(n);
        for (std::size_t pos = 0; pos < n; ++pos) {
            Part tmp = parts[pos];
            for (Rotate r : rots[pos]) {
                tmp.location.rotate = r;
                auto cells = tmp.rotatedCells();
                int mr = 0, mc = 0;
                for (auto [dr, dc] : cells) { if (dr > mr) mr = dr; if (dc > mc) mc = dc; }
                cellsByRot[pos].push_back(std::move(cells));
                extR[pos].push_back(mr);
                extC[pos].push_back(mc);
            }
        }
    }

    // Seed counts from the fixed (CANNOT_MOVE) cells and empties from EMPTY cells;
    // movable parts start unplaced.
    void initCounts() {
        const int C = static_cast<int>(board.colors);
        const int M = static_cast<int>(board.rows);
        const int N = static_cast<int>(board.cols);
        rowCnt.assign(C, std::vector<int>(M, 0));
        colCnt.assign(C, std::vector<int>(N, 0));
        rowEmpty.assign(M, 0);
        colEmpty.assign(N, 0);
        for (int r = 0; r < M; ++r)
            for (int c = 0; c < N; ++c) {
                const int cell = board._boardInfo[r][c];
                if (Board::isCannotMove(cell)) {
                    const int color = Board::cannotMoveColor(cell);
                    if (color >= 0 && color < C) { ++rowCnt[color][r]; ++colCnt[color][c]; }
                } else if (cell == Board::EMPTY) {
                    ++rowEmpty[r]; ++colEmpty[c];
                }
            }
    }

    // Add (sign=+1) or undo (sign=-1) part p's cells in the running counts. The
    // full delta is always applied first so a later sign=-1 call undoes it
    // exactly. On an add, sets exceeded=true if any line the part touches becomes
    // unsatisfiable — either it OVERSHOOTS a colour requirement, or its remaining
    // deficit for some colour can no longer fit in the empty cells left on that
    // line (a dead end the old rescan only discovered much deeper).
    void applyCounts(const std::vector<std::pair<int, int>>& cells,
                     int baseRow, int baseCol, int color, int sign, bool& exceeded) {
        const int C = static_cast<int>(board.colors);
        const int M = static_cast<int>(board.rows);
        for (auto [dr, dc] : cells) {
            const int rr = baseRow + dr;
            const int cc = baseCol + dc;
            rowCnt[color][rr] += sign;
            colCnt[color][cc] += sign;
            rowEmpty[rr] -= sign;   // placing (sign=+1) consumes an empty cell
            colEmpty[cc] -= sign;
        }
        if (sign < 0) return;       // undo can never create an infeasible line

        for (auto [dr, dc] : cells) {
            const int rr = baseRow + dr;
            const int cc = baseCol + dc;
            for (int c0 = 0; c0 < C; ++c0) {
                const int needR = static_cast<int>(board._constraints[c0][rr]) - rowCnt[c0][rr];
                if (needR < 0 || needR > rowEmpty[rr]) { exceeded = true; return; }
                const int needC = static_cast<int>(board._constraints[c0][M + cc]) - colCnt[c0][cc];
                if (needC < 0 || needC > colEmpty[cc]) { exceeded = true; return; }
            }
        }
    }

    bool backtrack(std::size_t k) {
        if (--budget < 0) return false;
        if (k == order.size()) return WinChecker::isWon(board, parts);

        const int pos = order[k];
        Part& p = parts[pos];
        const int M = static_cast<int>(board.rows);
        const int N = static_cast<int>(board.cols);
        const int color = static_cast<int>(p.colorIndex);
        const int mark  = static_cast<int>(Board::OCCUPIED) + static_cast<int>(p.partIndex);

        for (std::size_t j = 0; j < rots[pos].size(); ++j) {
            const auto& cells = cellsByRot[pos][j];
            const int maxRow = M - extR[pos][j];   // row+maxOffset stays < M
            const int maxCol = N - extC[pos][j];
            for (int row = 0; row < maxRow; ++row) {
                for (int col = 0; col < maxCol; ++col) {
                    // Inline canPlace: offsets are >=0 and the loop is trimmed to
                    // the rotated extent, so every cell is in-bounds already.
                    bool fits = true;
                    for (auto [dr, dc] : cells)
                        if (board._boardInfo[row + dr][col + dc] != Board::EMPTY) { fits = false; break; }
                    if (!fits) continue;

                    for (auto [dr, dc] : cells)
                        board._boardInfo[row + dr][col + dc] = static_cast<Board::BoardInfo>(mark);
                    p.location.row = row;
                    p.location.col = col;
                    p.location.rotate = rots[pos][j];
                    p.location.placed = true;

                    bool exceeded = false;
                    applyCounts(cells, row, col, color, +1, exceeded);
                    if (!exceeded) {
                        result[pos] = p.location;
                        if (backtrack(k + 1)) return true;
                    }
                    applyCounts(cells, row, col, color, -1, exceeded);   // undo counts
                    for (auto [dr, dc] : cells)
                        board._boardInfo[row + dr][col + dc] = Board::EMPTY;
                    p.location.placed = false;
                }
            }
        }
        return false;
    }
};

} // namespace

std::optional<std::vector<PartLocation>> Solver::solve(
    Board board, std::vector<Part> parts, long nodeBudget) {
    const std::size_t n = parts.size();

    Search s;
    s.board = std::move(board);
    s.parts = std::move(parts);
    s.budget = nodeBudget;
    s.result.resize(n);

    // Fast reject impossible levels (e.g. an editor export where a part was left
    // in the tray, so the derived constraints under-count that color) before
    // paying for a full backtracking sweep.
    if (!passesBalanceCheck(s.board, s.parts)) return std::nullopt;

    s.rots.resize(n);
    for (std::size_t i = 0; i < n; ++i) s.rots[i] = distinctRotations(s.parts[i]);

    // Place the largest parts first — fewer legal positions early means a
    // smaller search tree.
    s.order.resize(n);
    std::iota(s.order.begin(), s.order.end(), 0);
    std::sort(s.order.begin(), s.order.end(),
              [&](int a, int b) { return cellCount(s.parts[a]) > cellCount(s.parts[b]); });

    s.initCells();
    s.initCounts();
    if (s.backtrack(0)) return s.result;
    return std::nullopt;
}
