#ifndef ORIGINIUM_CORE_SOLVER_H
#define ORIGINIUM_CORE_SOLVER_H

#include <optional>
#include <vector>

#include "Board.h"
#include "Part.h"

// Pure backtracking solver. No raylib, no Game/UI dependency — safe to hand off
// as a standalone module. Reuses Board::canPlace/place/remove and the exact
// win condition (WinChecker::isWon) so the solver can never disagree with the
// live game's rules.
class Solver {
public:
    // Try to place every part in `parts` onto `board` so that all per-color
    // row/column constraints are met exactly (the same condition the game
    // checks for victory). Both arguments are taken BY VALUE — the caller's
    // board/parts are never modified.
    //
    // Returns one solution as a vector<PartLocation> aligned to `parts` by
    // index (result[i] is where parts[i] goes). Returns nullopt if no solution
    // exists or the search exceeds `nodeBudget` (guards against pathological
    // hangs on demo day).
    static std::optional<std::vector<PartLocation>> solve(
        Board board, std::vector<Part> parts, long nodeBudget = 2'000'000);
};

#endif
