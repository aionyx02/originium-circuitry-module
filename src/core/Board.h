#ifndef ORIGINIUM_CORE_BOARD_H
#define ORIGINIUM_CORE_BOARD_H

#include <vector>

#include "Part.h"

class Board {
public:
    unsigned colors = 0;
    unsigned rows = 0;
    unsigned cols = 0;

    enum BoardInfo : int {
        CANNOT_PLACE = -1,
        CANNOT_MOVE  = -2,
        OCCUPIED     = 1,
        EMPTY        = 0,
    };

    std::vector<std::vector<BoardInfo>> _boardInfo;
    std::vector<std::vector<unsigned>>  _constraints;

    bool inBounds(int row, int col) const;
    bool canPlace(const Part& p, int row, int col) const;
    void place(Part& p, int row, int col);
    void remove(Part& p);

    static bool isOccupied(int cell);
    static int  occupiedPartIndex(int cell);
    static bool isCannotMove(int cell);
    static int  cannotMoveColor(int cell);
};

#endif
