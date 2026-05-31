#include "WinChecker.h"

bool WinChecker::isWon(const Board& board, const std::vector<Part>& parts) {
    for (const auto& p : parts) {
        if (!p.location.placed) return false;
    }

    const int M = static_cast<int>(board.rows);
    const int N = static_cast<int>(board.cols);

    for (int color = 0; color < static_cast<int>(board.colors); ++color) {
        for (int r = 0; r < M; ++r) {
            const unsigned actual = board.currentFilledForColor(color, r, true, parts);
            if (actual != board._constraints[color][r]) return false;
        }
        for (int c = 0; c < N; ++c) {
            const unsigned actual = board.currentFilledForColor(color, c, false, parts);
            if (actual != board._constraints[color][M + c]) return false;
        }
    }
    return true;
}
