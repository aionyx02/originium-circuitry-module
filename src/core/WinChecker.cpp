#include "WinChecker.h"

namespace {

bool cellCountsForColor(int cell,
                        int color,
                        const std::vector<Part>& parts) {
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

bool WinChecker::isWon(const Board& board, const std::vector<Part>& parts) {
    for (const auto& p : parts) {
        if (!p.location.placed) return false;
    }

    const int M = static_cast<int>(board.rows);
    const int N = static_cast<int>(board.cols);

    for (int color = 0; color < static_cast<int>(board.colors); ++color) {
        for (int r = 0; r < M; ++r) {
            unsigned actual = 0;
            for (int c = 0; c < N; ++c) {
                if (cellCountsForColor(board._boardInfo[r][c], color, parts)) ++actual;
            }
            if (actual != board._constraints[color][r]) return false;
        }
        for (int c = 0; c < N; ++c) {
            unsigned actual = 0;
            for (int r = 0; r < M; ++r) {
                if (cellCountsForColor(board._boardInfo[r][c], color, parts)) ++actual;
            }
            if (actual != board._constraints[color][M + c]) return false;
        }
    }
    return true;
}
