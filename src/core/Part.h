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

    // Rotation/move pivot cell within un-rotated shape — guaranteed to be on
    // an occupied cell (closest to shape centroid). Set by computeCenterCell().
    int centerCellRow = 0;
    int centerCellCol = 0;

    // Phase 2 visual state — cosmetic only, not part of game logic.
    // currentAngle is monotonic (always grows CW with each rotation), avoiding
    // shortest-arc wrap bugs. visualInitialized skips first-frame snap.
    unsigned rotateCount     = 0;
    float    currentCenterX  = 0.0f;
    float    currentCenterY  = 0.0f;
    float    currentAngle    = 0.0f;
    float    currentScale    = 1.0f;
    bool     visualInitialized = false;

    void computeCenterCell();

    std::vector<std::vector<bool>> rotatedShape() const;
    std::vector<std::pair<int, int>> rotatedCells() const;
    int rotatedRows() const;
    int rotatedCols() const;
    void rotatedCenterCell(int& outRow, int& outCol) const;
};

#endif
