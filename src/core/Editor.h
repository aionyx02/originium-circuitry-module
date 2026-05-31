#ifndef ORIGINIUM_CORE_EDITOR_H
#define ORIGINIUM_CORE_EDITOR_H

#include <vector>

#include "Board.h"
#include "Part.h"

// Level editor data model. Pure core (no raylib): holds the Board + parts being
// designed and the editing operations on them. The UI layer drives these ops
// and renders the result; LevelWriter serializes it; Game plays it.
//
// Increment 1 covers board size, color count and row/column constraints.
// Blocked cells, fixed parts and arbitrary-shape part authoring come next.
class Editor {
public:
    static constexpr int kMaxDim    = 12;  // max rows/cols
    static constexpr int kMaxColors = 4;   // Renderer's palette has 4 colors

    Board board;
    std::vector<Part> parts;
    unsigned currentColor = 0;

    Editor();  // 3x3, 1 color, all-zero constraints, empty board

    void setSize(int rows, int cols);   // clamp to [1,kMaxDim], preserve overlap
    void setColorCount(int colors);     // clamp to [1,kMaxColors]
    // delta-adjust one row/column requirement for a color; clamps to a sane max
    // (a per-color row requirement can't exceed the column count, and vice versa).
    void adjustConstraint(unsigned color, int idx, bool isRow, int delta);

    unsigned constraintAt(unsigned color, int idx, bool isRow) const;
};

#endif
