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

// Prune: a line whose current per-color fill already EXCEEDS its requirement
// can never recover (we only add cells), so the branch is dead.
bool exceedsAnyConstraint(const Board& board, const std::vector<Part>& parts) {
    const int M = static_cast<int>(board.rows);
    const int N = static_cast<int>(board.cols);
    for (int color = 0; color < static_cast<int>(board.colors); ++color) {
        for (int r = 0; r < M; ++r)
            if (board.currentFilledForColor(color, r, true, parts)
                > board._constraints[color][r]) return true;
        for (int c = 0; c < N; ++c)
            if (board.currentFilledForColor(color, c, false, parts)
                > board._constraints[color][M + c]) return true;
    }
    return false;
}

struct Search {
    Board board;
    std::vector<Part> parts;
    std::vector<int> order;                 // part vector-indices, biggest first
    std::vector<std::vector<Rotate>> rots;  // distinct rotations per part index
    std::vector<PartLocation> result;       // aligned to parts by index
    long budget;

    bool backtrack(std::size_t k) {
        if (--budget < 0) return false;
        if (k == order.size()) return WinChecker::isWon(board, parts);

        const int pos = order[k];
        Part& p = parts[pos];
        const int M = static_cast<int>(board.rows);
        const int N = static_cast<int>(board.cols);

        for (Rotate rot : rots[pos]) {
            p.location.rotate = rot;
            for (int row = 0; row < M; ++row) {
                for (int col = 0; col < N; ++col) {
                    if (!board.canPlace(p, row, col)) continue;
                    board.place(p, row, col);           // sets p.location + placed
                    if (!exceedsAnyConstraint(board, parts)) {
                        result[pos] = p.location;
                        if (backtrack(k + 1)) return true;
                    }
                    board.remove(p);                    // clears placed
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
    s.rots.resize(n);
    for (std::size_t i = 0; i < n; ++i) s.rots[i] = distinctRotations(s.parts[i]);

    // Place the largest parts first — fewer legal positions early means a
    // smaller search tree.
    s.order.resize(n);
    std::iota(s.order.begin(), s.order.end(), 0);
    std::sort(s.order.begin(), s.order.end(),
              [&](int a, int b) { return cellCount(s.parts[a]) > cellCount(s.parts[b]); });

    if (s.backtrack(0)) return s.result;
    return std::nullopt;
}
