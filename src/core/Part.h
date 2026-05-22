#ifndef ORIGINIUM_CORE_PART_H
#define ORIGINIUM_CORE_PART_H

#include <utility>
#include <vector>

enum class Rotate {
    CW_0 = 0,
    CW_90 = 1,
    CW_180 = 2,
    CW_270 = 3,
};

struct PartLocation {
    int row;
    int col;
    Rotate rotate;
    bool placed;

    PartLocation() : row(0), col(0), rotate(Rotate::CW_0), placed(false) {}
};

class Part {
public:
    unsigned colorIndex = 0;
    unsigned partIndex  = 0;
    std::vector<std::vector<bool>> shape;
    PartLocation location;

    std::vector<std::vector<bool>> rotatedShape() const;
    std::vector<std::pair<int, int>> rotatedCells() const;
    int rotatedRows() const;
    int rotatedCols() const;
};

#endif
