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

    // Board cells: new grid of EMPTY, preserving the overlapping top-left region.
    std::vector<std::vector<Board::BoardInfo>> newInfo(
        rows, std::vector<Board::BoardInfo>(cols, Board::EMPTY));
    for (int r = 0; r < std::min(oldM, rows); ++r)
        for (int c = 0; c < std::min(oldN, cols); ++c)
            newInfo[r][c] = board._boardInfo[r][c];

    // Constraints: per color, M row reqs then N col reqs; preserve overlap.
    std::vector<std::vector<unsigned>> newCons(C, std::vector<unsigned>(rows + cols, 0));
    for (int color = 0; color < C; ++color) {
        if (color < static_cast<int>(board._constraints.size())) {
            for (int r = 0; r < std::min(oldM, rows); ++r)
                newCons[color][r] = board._constraints[color][r];
            for (int c = 0; c < std::min(oldN, cols); ++c)
                newCons[color][rows + c] = board._constraints[color][oldM + c];
        }
    }

    board.rows = static_cast<unsigned>(rows);
    board.cols = static_cast<unsigned>(cols);
    board._boardInfo = std::move(newInfo);
    board._constraints = std::move(newCons);
}

void Editor::setColorCount(int colors) {
    colors = std::max(1, std::min(colors, kMaxColors));
    const int M = static_cast<int>(board.rows);
    const int N = static_cast<int>(board.cols);

    // Drop fixed cells whose color no longer exists.
    for (auto& row : board._boardInfo)
        for (auto& cell : row)
            if (Board::isCannotMove(cell) && Board::cannotMoveColor(cell) >= colors)
                cell = Board::EMPTY;

    board._constraints.resize(colors, std::vector<unsigned>(M + N, 0));
    board.colors = static_cast<unsigned>(colors);
    if (currentColor >= static_cast<unsigned>(colors))
        currentColor = static_cast<unsigned>(colors - 1);
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
    if (!parts.empty()) parts.pop_back();
}

unsigned Editor::constraintAt(unsigned color, int idx, bool isRow) const {
    if (color >= board.colors) return 0;
    const int M = static_cast<int>(board.rows);
    const int pos = isRow ? idx : (M + idx);
    if (pos < 0 || pos >= static_cast<int>(board._constraints[color].size())) return 0;
    return board._constraints[color][pos];
}
