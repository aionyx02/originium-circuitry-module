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

    void init(Board b, std::vector<Part> p);
    void update(Action a);

    static constexpr int TRAY_COL = -1;

private:
    void handlePlace();
    void handleRemove();
    void clampCursor();
    void syncHeldLocation();
    void autoSelectNextUnplaced();
};

#endif
