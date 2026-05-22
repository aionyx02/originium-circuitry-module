#include "Part.h"

#include <limits>

void Part::computeCenterCell() {
    const int M = static_cast<int>(shape.size());
    const int N = M > 0 ? static_cast<int>(shape[0].size()) : 0;

    // Centroid of occupied cells
    int count = 0;
    float sumR = 0.0f, sumC = 0.0f;
    for (int r = 0; r < M; ++r) {
        for (int c = 0; c < N; ++c) {
            if (shape[r][c]) { sumR += r; sumC += c; ++count; }
        }
    }
    if (count == 0) { centerCellRow = 0; centerCellCol = 0; return; }
    const float avgR = sumR / count;
    const float avgC = sumC / count;

    // Pick occupied cell closest to centroid (squared distance)
    float bestD = std::numeric_limits<float>::infinity();
    for (int r = 0; r < M; ++r) {
        for (int c = 0; c < N; ++c) {
            if (!shape[r][c]) continue;
            const float dr = r - avgR;
            const float dc = c - avgC;
            const float d = dr * dr + dc * dc;
            if (d < bestD) { bestD = d; centerCellRow = r; centerCellCol = c; }
        }
    }
}

void Part::rotatedCenterCell(int& outRow, int& outCol) const {
    const int M = static_cast<int>(shape.size());
    const int N = M > 0 ? static_cast<int>(shape[0].size()) : 0;
    const int k = static_cast<int>(location.rotate) % 4;
    const int cr = centerCellRow;
    const int cc = centerCellCol;
    switch (k) {
        case 0: outRow = cr;          outCol = cc;          break;
        case 1: outRow = cc;          outCol = M - 1 - cr;  break;
        case 2: outRow = M - 1 - cr;  outCol = N - 1 - cc;  break;
        case 3: outRow = N - 1 - cc;  outCol = cr;          break;
    }
}

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
