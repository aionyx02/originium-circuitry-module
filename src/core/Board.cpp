#include "Board.h"

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
