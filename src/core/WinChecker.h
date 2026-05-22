#ifndef ORIGINIUM_CORE_WINCHECKER_H
#define ORIGINIUM_CORE_WINCHECKER_H

#include <vector>

#include "Board.h"
#include "Part.h"

class WinChecker {
public:
    static bool isWon(const Board& board, const std::vector<Part>& parts);
};

#endif
