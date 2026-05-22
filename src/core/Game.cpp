#include "Game.h"

#include <algorithm>

#include "WinChecker.h"

void Game::init(Board b, std::vector<Part> p) {
    board = std::move(b);
    parts = std::move(p);
    cursorRow = 0;
    cursorCol = TRAY_COL;
    heldPartIdx = -1;
    won = false;
    statusMessage.clear();
    autoSelectNextUnplaced();
}

void Game::update(Action a) {
    statusMessage.clear();
    if (won) return;

    switch (a) {
        case Action::MoveUp:    --cursorRow; break;
        case Action::MoveDown:  ++cursorRow; break;
        case Action::MoveLeft:  --cursorCol; break;
        case Action::MoveRight: ++cursorCol; break;
        case Action::Rotate: {
            if (heldPartIdx >= 0) {
                auto& p = parts[heldPartIdx];
                p.location.rotate = static_cast<Rotate>(
                    (static_cast<int>(p.location.rotate) + 1) % 4);
            }
            break;
        }
        case Action::Place:  handlePlace();  break;
        case Action::Remove: handleRemove(); break;
        case Action::None: break;
    }

    clampCursor();
    syncHeldLocation();

    if (!won) won = WinChecker::isWon(board, parts);
}

void Game::clampCursor() {
    const int minCol = TRAY_COL;
    const int maxCol = static_cast<int>(board.cols) - 1;
    cursorCol = std::max(minCol, std::min(cursorCol, maxCol));

    int maxRow;
    if (cursorCol == TRAY_COL) {
        const int trayRows = static_cast<int>(parts.size());
        maxRow = std::max(trayRows, static_cast<int>(board.rows)) - 1;
    } else {
        maxRow = static_cast<int>(board.rows) - 1;
    }
    if (maxRow < 0) maxRow = 0;
    cursorRow = std::max(0, std::min(cursorRow, maxRow));
}

void Game::syncHeldLocation() {
    if (heldPartIdx < 0) return;
    auto& p = parts[heldPartIdx];
    p.location.row = cursorRow;
    p.location.col = cursorCol;
}

void Game::autoSelectNextUnplaced() {
    if (heldPartIdx >= 0) return;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (!parts[i].location.placed) {
            heldPartIdx = static_cast<int>(i);
            cursorCol   = TRAY_COL;
            cursorRow   = static_cast<int>(i);
            return;
        }
    }
}

void Game::handlePlace() {
    if (heldPartIdx >= 0) {
        if (cursorCol < 0) {
            statusMessage = "Move onto the board (press D) before pressing Enter.";
            return;
        }
        Part& p = parts[heldPartIdx];
        if (!board.canPlace(p, cursorRow, cursorCol)) {
            statusMessage = "Cannot place here: out of bounds, overlap, or blocked.";
            return;
        }
        board.place(p, cursorRow, cursorCol);
        const int placedIdx = heldPartIdx;
        heldPartIdx = -1;
        autoSelectNextUnplaced();
        if (heldPartIdx < 0) {
            statusMessage = "All parts placed.";
        }
        (void)placedIdx;
        return;
    }

    if (cursorCol == TRAY_COL) {
        if (cursorRow >= 0 && cursorRow < static_cast<int>(parts.size())
            && !parts[cursorRow].location.placed) {
            heldPartIdx = cursorRow;
        }
        return;
    }

    const int cell = board._boardInfo[cursorRow][cursorCol];
    if (Board::isOccupied(cell)) {
        const int partIdx = Board::occupiedPartIndex(cell);
        if (partIdx >= 0 && partIdx < static_cast<int>(parts.size())) {
            board.remove(parts[partIdx]);
            heldPartIdx = partIdx;
        }
    }
}

void Game::handleRemove() {
    if (heldPartIdx >= 0) {
        Part& p = parts[heldPartIdx];
        p.location.row = static_cast<int>(p.partIndex);
        p.location.col = TRAY_COL;
        heldPartIdx = -1;
        return;
    }

    if (cursorCol == TRAY_COL) return;

    const int cell = board._boardInfo[cursorRow][cursorCol];
    if (Board::isOccupied(cell)) {
        const int partIdx = Board::occupiedPartIndex(cell);
        if (partIdx >= 0 && partIdx < static_cast<int>(parts.size())) {
            board.remove(parts[partIdx]);
        }
    }
}
