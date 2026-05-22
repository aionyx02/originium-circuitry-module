#include "Part.h"

int Part::rotatedRows() const {
    const int M = static_cast<int>(shape.size());
    const int N = M > 0 ? static_cast<int>(shape[0].size()) : 0;
    const int k = static_cast<int>(location.rotate) % 4;
    return (k == 0 || k == 2) ? M : N;
}

int Part::rotatedCols() const {
    const int M = static_cast<int>(shape.size());
    const int N = M > 0 ? static_cast<int>(shape[0].size()) : 0;
    const int k = static_cast<int>(location.rotate) % 4;
    return (k == 0 || k == 2) ? N : M;
}

std::vector<std::vector<bool>> Part::rotatedShape() const {
    const int M = static_cast<int>(shape.size());
    const int N = M > 0 ? static_cast<int>(shape[0].size()) : 0;
    const int k = static_cast<int>(location.rotate) % 4;

    const int newRows = rotatedRows();
    const int newCols = rotatedCols();
    std::vector<std::vector<bool>> out(newRows, std::vector<bool>(newCols, false));

    for (int r = 0; r < M; ++r) {
        for (int c = 0; c < N; ++c) {
            if (!shape[r][c]) continue;
            int nr = 0, nc = 0;
            switch (k) {
                case 0: nr = r;             nc = c;             break;
                case 1: nr = c;             nc = M - 1 - r;     break;
                case 2: nr = M - 1 - r;     nc = N - 1 - c;     break;
                case 3: nr = N - 1 - c;     nc = r;             break;
            }
            out[nr][nc] = true;
        }
    }
    return out;
}

std::vector<std::pair<int, int>> Part::rotatedCells() const {
    const auto rs = rotatedShape();
    std::vector<std::pair<int, int>> cells;
    cells.reserve(rs.size() * (rs.empty() ? 0 : rs[0].size()));
    for (int r = 0; r < static_cast<int>(rs.size()); ++r) {
        for (int c = 0; c < static_cast<int>(rs[r].size()); ++c) {
            if (rs[r][c]) cells.emplace_back(r, c);
        }
    }
    return cells;
}
