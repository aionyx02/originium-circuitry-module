#ifndef ORIGINIUM_CORE_GAME_H
#define ORIGINIUM_CORE_GAME_H

#include <string>
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

    static constexpr int TRAY_COL = -1;

private:
    // Snapshot of board + parts as init() received them. Used by
    // resetToInitial() — copied (not moved) so reset can run repeatedly.
    Board initialBoard;
    std::vector<Part> initialParts;

    void handlePlace();
    void handleRemove();
    void clampCursor();
    void syncHeldLocation();
};

#endif
