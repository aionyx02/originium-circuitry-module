#include "Editor.h"

#include <algorithm>

Editor::Editor() {
    setColorCount(1);
    setSize(3, 3);
}

void Editor::setSize(int rows, int cols) {
    rows = std::max(1, std::min(rows, kMaxDim));
    cols = std::max(1, std::min(cols, kMaxDim));

    const int oldM = static_cast<int>(board.rows);
    const int oldN = static_cast<int>(board.cols);
    const int C    = std::max(1, static_cast<int>(board.colors));

    // Rebuild the grid. Preserve only blocked / fixed cells in the overlap;
    // any placed piece is lifted (the layout is re-derived from scratch).
    std::vector<std::vector<Board::BoardInfo>> newInfo(
        rows, std::vector<Board::BoardInfo>(cols, Board::EMPTY));
    for (int r = 0; r < std::min(oldM, rows); ++r) {
        for (int c = 0; c < std::min(oldN, cols); ++c) {
            const Board::BoardInfo v = board._boardInfo[r][c];
            if (v == Board::CANNOT_PLACE || Board::isCannotMove(v)) newInfo[r][c] = v;
        }
    }
    for (auto& p : parts) p.location.placed = false;  // board cleared of pieces

    board.rows = static_cast<unsigned>(rows);
    board.cols = static_cast<unsigned>(cols);
    board._boardInfo = std::move(newInfo);
    board._constraints.assign(C, std::vector<unsigned>(rows + cols, 0));
    deriveConstraints();
}

void Editor::setColorCount(int colors) {
    colors = std::max(1, std::min(colors, kMaxColors));
    const int M = static_cast<int>(board.rows);
    const int N = static_cast<int>(board.cols);

    // Drop fixed cells whose color no longer exists; clamp piece colors.
    for (auto& row : board._boardInfo)
        for (auto& cell : row)
            if (Board::isCannotMove(cell) && Board::cannotMoveColor(cell) >= colors)
                cell = Board::EMPTY;
    for (auto& p : parts)
        if (p.colorIndex >= static_cast<unsigned>(colors))
            p.colorIndex = static_cast<unsigned>(colors - 1);

    board._constraints.resize(colors, std::vector<unsigned>(M + N, 0));
    for (auto& c : board._constraints) c.assign(M + N, 0);
    board.colors = static_cast<unsigned>(colors);
    if (currentColor >= static_cast<unsigned>(colors))
        currentColor = static_cast<unsigned>(colors - 1);
    deriveConstraints();
}

bool Editor::placePart(int idx, int row, int col) {
    if (idx < 0 || idx >= static_cast<int>(parts.size())) return false;
    if (parts[idx].location.placed) return false;
    if (!board.canPlace(parts[idx], row, col)) return false;
    board.place(parts[idx], row, col);  // writes OCCUPIED + partIndex (== idx)
    deriveConstraints();
    return true;
}

void Editor::removePartAt(int row, int col) {
    if (!board.inBounds(row, col)) return;
    const int cell = board._boardInfo[row][col];
    if (!Board::isOccupied(cell)) return;
    const int pi = Board::occupiedPartIndex(cell);
    if (pi >= 0 && pi < static_cast<int>(parts.size())) {
        board.remove(parts[pi]);
        deriveConstraints();
    }
}

void Editor::rotatePart(int idx) {
    if (idx < 0 || idx >= static_cast<int>(parts.size())) return;
    if (parts[idx].location.placed) return;  // rotate before placing
    Part& p = parts[idx];
    p.location.rotate = static_cast<Rotate>((static_cast<int>(p.location.rotate) + 1) % 4);
}

void Editor::deriveConstraints() {
    // Row/column requirements are exactly what the current placed layout
    // (placed pieces + fixed cells) fills — so the puzzle is always solvable
    // by reproducing this layout.
    const int M = static_cast<int>(board.rows);
    const int N = static_cast<int>(board.cols);
    for (int color = 0; color < static_cast<int>(board.colors); ++color) {
        for (int r = 0; r < M; ++r)
            board._constraints[color][r] = board.currentFilledForColor(color, r, true, parts);
        for (int c = 0; c < N; ++c)
            board._constraints[color][M + c] = board.currentFilledForColor(color, c, false, parts);
    }
}

void Editor::adjustConstraint(unsigned color, int idx, bool isRow, int delta) {
    if (color >= board.colors) return;
    const int M = static_cast<int>(board.rows);
    const int N = static_cast<int>(board.cols);
    const int limit = isRow ? N : M;            // cells available along that line
    const int span  = isRow ? M : N;
    if (idx < 0 || idx >= span) return;

    const int pos = isRow ? idx : (M + idx);
    const int cur = static_cast<int>(board._constraints[color][pos]);
    board._constraints[color][pos] =
        static_cast<unsigned>(std::max(0, std::min(cur + delta, limit)));
}

void Editor::addPart(const std::vector<std::vector<bool>>& mask, unsigned color) {
    int minR = -1, minC = -1, maxR = -1, maxC = -1;
    for (int r = 0; r < static_cast<int>(mask.size()); ++r) {
        for (int c = 0; c < static_cast<int>(mask[r].size()); ++c) {
            if (!mask[r][c]) continue;
            if (minR < 0 || r < minR) minR = r;
            if (minC < 0 || c < minC) minC = c;
            if (r > maxR) maxR = r;
            if (c > maxC) maxC = c;
        }
    }
    if (maxR < 0) return;  // empty mask

    Part p;
    p.colorIndex = std::min(color, board.colors > 0 ? board.colors - 1 : 0u);
    p.partIndex  = static_cast<unsigned>(parts.size());
    p.shape.assign(maxR - minR + 1, std::vector<bool>(maxC - minC + 1, false));
    for (int r = minR; r <= maxR; ++r)
        for (int c = minC; c <= maxC; ++c)
            p.shape[r - minR][c - minC] = mask[r][c];
    p.computeCenterCell();
    parts.push_back(std::move(p));
}

void Editor::removeLastPart() {
    if (parts.empty()) return;
    if (parts.back().location.placed) board.remove(parts.back());
    parts.pop_back();
    deriveConstraints();
}

void Editor::setBlocked(int row, int col) {
    if (!board.inBounds(row, col)) return;
    if (Board::isOccupied(board._boardInfo[row][col])) removePartAt(row, col);
    board._boardInfo[row][col] = Board::CANNOT_PLACE;
    deriveConstraints();
}

void Editor::setFixed(int row, int col, unsigned color) {
    if (!board.inBounds(row, col)) return;
    if (board.colors == 0) return;
    if (Board::isOccupied(board._boardInfo[row][col])) removePartAt(row, col);
    const int c = static_cast<int>(std::min(color, board.colors - 1));
    board._boardInfo[row][col] =
        static_cast<Board::BoardInfo>(static_cast<int>(Board::CANNOT_MOVE) - c);
    deriveConstraints();
}

void Editor::clearCell(int row, int col) {
    if (!board.inBounds(row, col)) return;
    if (Board::isOccupied(board._boardInfo[row][col])) { removePartAt(row, col); return; }
    board._boardInfo[row][col] = Board::EMPTY;
    deriveConstraints();
}

unsigned Editor::constraintAt(unsigned color, int idx, bool isRow) const {
    if (color >= board.colors) return 0;
    const int M = static_cast<int>(board.rows);
    const int pos = isRow ? idx : (M + idx);
    if (pos < 0 || pos >= static_cast<int>(board._constraints[color].size())) return 0;
    return board._constraints[color][pos];
}
