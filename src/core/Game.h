#ifndef ORIGINIUM_CORE_GAME_H
#define ORIGINIUM_CORE_GAME_H

#include <string>
#include <utility>
#include <vector>

#include "Board.h"
#include "Part.h"

enum class Action {
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Rotate,
    Place,
    Remove,
    Reset,
    Solve,
};

class Game {
public:
    Board board;
    std::vector<Part> parts;

    int cursorRow    = 0;
    int cursorCol    = -1;
    int heldPartIdx  = -1;
    bool won         = false;
    std::string statusMessage;

    // UI hint: true when the last input that moved the cursor was the mouse.
    // Lets Renderer switch the held-part visual target from cell-center to
    // mouse pixel (pixel-perfect drag). Cleared on any keyboard Move*.
    bool mouseControlling = false;

    // Phase 4 increment 2 — solution hint overlay. The 30s idle timer lives in
    // the main loop; it toggles this via setHintsVisible(). When true, Renderer
    // tints the still-empty cells that the solution fills.
    bool hintsVisible = false;

    void init(Board b, std::vector<Part> p);
    void update(Action a);

    // Restore board + parts + transient state to what init() received.
    // Same-level restart; does not re-parse the file.
    void resetToInitial();

    // Snap cursor to an absolute target. If isTray, col is ignored and cursor
    // sits in the tray column (TRAY_COL). Clamps to valid range and re-syncs
    // any held part's location. Used by mouse input; keyboard still goes
    // through Action::Move* in update().
    void setCursor(int row, int col, bool isTray);

    // Show/hide the solution hint overlay. Showing lazily computes (and caches)
    // the solution the first time; if the level is unsolvable, stays hidden.
    void setHintsVisible(bool visible);

    // Cells the cached solution fills that are still EMPTY on the live board —
    // i.e. where the player still needs to place parts. Empty unless hints are
    // visible. Recomputed cheaply each call from the cached solution. Returns
    // only the FIRST still-unfilled solution part (one piece at a time).
    std::vector<std::pair<int, int>> hintCells() const;

    // Drive the step-by-step auto-solve animation. Called every frame by the
    // main loop; places the next solution part once kSolveStepInterval elapses,
    // letting the renderer lerp each part in from the tray. No-op when idle.
    void advanceSolve(float dt);

    // True while the auto-solve animation is mid-flight. The main loop locks
    // player input (except Reset) during this window.
    bool isSolving() const { return solving_; }

    static constexpr int TRAY_COL = -1;

private:
    // Snapshot of board + parts as init() received them. Used by
    // resetToInitial() — copied (not moved) so reset can run repeatedly.
    Board initialBoard;
    std::vector<Part> initialParts;

    // Cached full solution (from initialBoard/initialParts), computed lazily on
    // first hint/solve request. solutionComputed_ guards recomputation.
    bool solutionComputed_ = false;
    bool solutionExists_   = false;
    std::vector<PartLocation> solution_;
    void ensureSolution();

    // Step-by-step auto-solve animation state. solving_ is true between the F
    // press and the last part landing; solveStep_ is the next part index to
    // drop; solveTimer_ accumulates dt until the next step interval.
    bool  solving_    = false;
    int   solveStep_  = 0;
    float solveTimer_ = 0.0f;

    void handlePlace();
    void handleRemove();
    void solveAndApply();
    void clampCursor();
    void syncHeldLocation();
};

#endif
