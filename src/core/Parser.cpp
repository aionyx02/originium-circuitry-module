#include "Parser.h"

#include <sstream>

namespace {

template <typename T>
T readToken(std::istream& in, const char* what) {
    T v{};
    if (!(in >> v)) {
        std::ostringstream oss;
        oss << "parse error: expected " << what;
        throw Parser::Error(oss.str());
    }
    return v;
}

} // namespace

GameData Parser::parse(std::istream& in) {
    GameData data;
    Board& board = data.board;
    std::vector<Part>& parts = data.parts;

    const int C = readToken<int>(in, "color count C");
    const int M = readToken<int>(in, "row count M");
    const int N = readToken<int>(in, "col count N");
    if (C <= 0 || M <= 0 || N <= 0) {
        throw Error("parse error: C/M/N must be positive");
    }

    board.colors = static_cast<unsigned>(C);
    board.rows   = static_cast<unsigned>(M);
    board.cols   = static_cast<unsigned>(N);
    board._boardInfo.assign(M, std::vector<Board::BoardInfo>(N, Board::EMPTY));
    board._constraints.assign(C, std::vector<unsigned>(M + N, 0));

    for (int color = 0; color < C; ++color) {
        for (int r = 0; r < M; ++r) {
            board._constraints[color][r] = readToken<unsigned>(in, "row constraint");
        }
        for (int c = 0; c < N; ++c) {
            board._constraints[color][M + c] = readToken<unsigned>(in, "col constraint");
        }

        const int X1 = readToken<int>(in, "fixed-part count X1");
        for (int i = 0; i < X1; ++i) {
            const int r = readToken<int>(in, "fixed-part row");
            const int c = readToken<int>(in, "fixed-part col");
            if (!board.inBounds(r, c)) {
                throw Error("parse error: fixed-part coord out of bounds");
            }
            board._boardInfo[r][c] = static_cast<Board::BoardInfo>(
                static_cast<int>(Board::CANNOT_MOVE) - color);
        }
    }

    const int X2 = readToken<int>(in, "blocked-cell count X2");
    for (int i = 0; i < X2; ++i) {
        const int r = readToken<int>(in, "blocked-cell row");
        const int c = readToken<int>(in, "blocked-cell col");
        if (!board.inBounds(r, c)) {
            throw Error("parse error: blocked coord out of bounds");
        }
        board._boardInfo[r][c] = Board::CANNOT_PLACE;
    }

    while (true) {
        int colorIdx;
        if (!(in >> colorIdx)) break;
        const int m2 = readToken<int>(in, "part rows M2");
        const int n2 = readToken<int>(in, "part cols N2");
        if (m2 <= 0 || n2 <= 0) {
            throw Error("parse error: part dims must be positive");
        }
        if (colorIdx < 0 || colorIdx >= C) {
            throw Error("parse error: part color index out of range");
        }

        Part p;
        p.colorIndex = static_cast<unsigned>(colorIdx);
        p.partIndex  = static_cast<unsigned>(parts.size());
        p.shape.assign(m2, std::vector<bool>(n2, false));
        for (int r = 0; r < m2; ++r) {
            for (int c = 0; c < n2; ++c) {
                const int v = readToken<int>(in, "part cell 0/1");
                p.shape[r][c] = (v != 0);
            }
        }
        parts.push_back(std::move(p));
    }

    return data;
}
