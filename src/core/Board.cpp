#include "Board.h"

namespace {

bool cellCountsForColor(int cell, int color, const std::vector<Part>& parts) {
    if (Board::isCannotMove(cell)) {
        return Board::cannotMoveColor(cell) == color;
    }
    if (Board::isOccupied(cell)) {
        const int partIdx = Board::occupiedPartIndex(cell);
        if (partIdx >= 0 && partIdx < static_cast<int>(parts.size())) {
            return static_cast<int>(parts[partIdx].colorIndex) == color;
        }
    }
    return false;
}

} // namespace

bool Board::inBounds(int row, int col) const {
    return row >= 0 && col >= 0
        && row < static_cast<int>(rows)
        && col < static_cast<int>(cols);
}

bool Board::canPlace(const Part& p, int row, int col) const {
    for (auto [dr, dc] : p.rotatedCells()) {
        const int rr = row + dr;
        const int cc = col + dc;
        if (!inBounds(rr, cc)) return false;
        if (_boardInfo[rr][cc] != EMPTY) return false;
    }
    return true;
}

unsigned Board::currentFilledForColor(int color, int idx, bool isRow,
                                      const std::vector<Part>& parts) const {
    if (color < 0 || color >= static_cast<int>(colors)) return 0;

    unsigned total = 0;
    if (isRow) {
        if (idx < 0 || idx >= static_cast<int>(rows)) return 0;
        for (int c = 0; c < static_cast<int>(cols); ++c) {
            if (cellCountsForColor(_boardInfo[idx][c], color, parts)) ++total;
        }
    } else {
        if (idx < 0 || idx >= static_cast<int>(cols)) return 0;
        for (int r = 0; r < static_cast<int>(rows); ++r) {
            if (cellCountsForColor(_boardInfo[r][idx], color, parts)) ++total;
        }
    }
    return total;
}

void Board::place(Part& p, int row, int col) {
    for (auto [dr, dc] : p.rotatedCells()) {
        const int rr = row + dr;
        const int cc = col + dc;
        _boardInfo[rr][cc] = static_cast<BoardInfo>(OCCUPIED + static_cast<int>(p.partIndex));
    }
    p.location.row = row;
    p.location.col = col;
    p.location.placed = true;
}

void Board::remove(Part& p) {
    if (!p.location.placed) return;
    for (auto [dr, dc] : p.rotatedCells()) {
        const int rr = p.location.row + dr;
        const int cc = p.location.col + dc;
        if (inBounds(rr, cc)) _boardInfo[rr][cc] = EMPTY;
    }
    p.location.placed = false;
}

bool Board::isOccupied(int cell) {
    return cell >= static_cast<int>(OCCUPIED);
}

int Board::occupiedPartIndex(int cell) {
    return cell - static_cast<int>(OCCUPIED);
}

bool Board::isCannotMove(int cell) {
    return cell <= static_cast<int>(CANNOT_MOVE);
}

int Board::cannotMoveColor(int cell) {
    return static_cast<int>(CANNOT_MOVE) - cell;
}
