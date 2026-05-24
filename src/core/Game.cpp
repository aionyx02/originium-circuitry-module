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
    mouseControlling = false;
}

void Game::update(Action a) {
    statusMessage.clear();
    if (won) return;

    switch (a) {
        case Action::MoveUp:    --cursorRow; mouseControlling = false; break;
        case Action::MoveDown:  ++cursorRow; mouseControlling = false; break;
        case Action::MoveLeft:  --cursorCol; mouseControlling = false; break;
        case Action::MoveRight: ++cursorCol; mouseControlling = false; break;
        case Action::Rotate: {
            if (heldPartIdx >= 0) {
                auto& p = parts[heldPartIdx];
                p.location.rotate = static_cast<Rotate>(
                    (static_cast<int>(p.location.rotate) + 1) % 4);
                ++p.rotateCount;
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

void Game::setCursor(int row, int col, bool isTray) {
    cursorRow = row;
    cursorCol = isTray ? TRAY_COL : col;
    clampCursor();
    syncHeldLocation();
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
    // Cursor points to the pivot cell's target. Derive bbox top-left so the
    // pivot lands at (cursorRow, cursorCol) under the current rotation.
    if (cursorCol == TRAY_COL) {
        p.location.row = cursorRow;
        p.location.col = cursorCol;
    } else {
        int rCr, rCc;
        p.rotatedCenterCell(rCr, rCc);
        p.location.row = cursorRow - rCr;
        p.location.col = cursorCol - rCc;
    }
}

void Game::handlePlace() {
    if (heldPartIdx >= 0) {
        if (cursorCol < 0) {
            statusMessage = "Move onto the board (press D) before pressing Enter.";
            return;
        }
        Part& p = parts[heldPartIdx];
        // location.row/col was synced from cursor minus rotated pivot offset
        if (!board.canPlace(p, p.location.row, p.location.col)) {
            statusMessage = "Cannot place here: out of bounds, overlap, or blocked.";
            return;
        }
        board.place(p, p.location.row, p.location.col);
        heldPartIdx = -1;
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
            Part& p = parts[partIdx];
            // Snap cursor to the pivot cell so syncHeldLocation reproduces
            // the same bbox top-left and the part doesn't visually shift.
            int rCr, rCc;
            p.rotatedCenterCell(rCr, rCc);
            cursorRow = p.location.row + rCr;
            cursorCol = p.location.col + rCc;
            board.remove(p);
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
