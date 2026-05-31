#include "LevelWriter.h"

#include <sstream>

std::string LevelWriter::write(const Board& board, const std::vector<Part>& parts) {
    const int C = static_cast<int>(board.colors);
    const int M = static_cast<int>(board.rows);
    const int N = static_cast<int>(board.cols);

    std::ostringstream out;
    out << C << ' ' << M << ' ' << N << '\n';

    for (int color = 0; color < C; ++color) {
        // M row requirements, then N column requirements.
        for (int r = 0; r < M; ++r) {
            out << board._constraints[color][r] << (r + 1 < M ? ' ' : '\n');
        }
        for (int c = 0; c < N; ++c) {
            out << board._constraints[color][M + c] << (c + 1 < N ? ' ' : '\n');
        }

        // Fixed (CANNOT_MOVE) cells belonging to this color.
        std::ostringstream fixed;
        int x1 = 0;
        for (int r = 0; r < M; ++r) {
            for (int c = 0; c < N; ++c) {
                const int cell = board._boardInfo[r][c];
                if (Board::isCannotMove(cell) && Board::cannotMoveColor(cell) == color) {
                    fixed << r << ' ' << c << '\n';
                    ++x1;
                }
            }
        }
        out << x1 << '\n' << fixed.str();
    }

    // Blocked (CANNOT_PLACE) cells — shared across colors, after all color blocks.
    std::ostringstream blocked;
    int x2 = 0;
    for (int r = 0; r < M; ++r) {
        for (int c = 0; c < N; ++c) {
            if (board._boardInfo[r][c] == Board::CANNOT_PLACE) {
                blocked << r << ' ' << c << '\n';
                ++x2;
            }
        }
    }
    out << x2 << '\n' << blocked.str();

    // Parts: "colorIndex M2 N2" then the 0/1 shape rows.
    for (const auto& p : parts) {
        const int m2 = static_cast<int>(p.shape.size());
        const int n2 = m2 > 0 ? static_cast<int>(p.shape[0].size()) : 0;
        out << p.colorIndex << ' ' << m2 << ' ' << n2 << '\n';
        for (int r = 0; r < m2; ++r) {
            for (int c = 0; c < n2; ++c) {
                out << (p.shape[r][c] ? 1 : 0) << (c + 1 < n2 ? ' ' : '\n');
            }
        }
    }

    return out.str();
}
