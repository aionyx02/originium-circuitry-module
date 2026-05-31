#include "Game.h"

#include <algorithm>

#include "Solver.h"
#include "WinChecker.h"

void Game::init(Board b, std::vector<Part> p) {
    board = std::move(b);
    parts = std::move(p);
    // Snapshot for resetToInitial(). Taken after move so initialParts mirrors
    // current parts exactly, with all Part visual fields still at defaults
    // (visualInitialized=false, rotateCount=0, currentAngle=0, currentScale=1).
    initialBoard = board;
    initialParts = parts;
    cursorRow = 0;
    cursorCol = TRAY_COL;
    heldPartIdx = -1;
    won = false;
    statusMessage.clear();
    mouseControlling = false;
}

void Game::resetToInitial() {
    board = initialBoard;
    parts = initialParts;
    cursorRow = 0;
    cursorCol = TRAY_COL;
    heldPartIdx = -1;
    won = false;
    mouseControlling = false;
    statusMessage = "Board reset.";
}

void Game::update(Action a) {
    statusMessage.clear();
    // Reset must work even after winning (player wants to replay the same level).
    if (a == Action::Reset) {
        resetToInitial();
        return;
    }
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
        case Action::Solve:  solveAndApply(); break;
        case Action::Reset:  break;  // handled above; listed to satisfy -Wswitch
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

void Game::solveAndApply() {
    // Always solve from the clean initial snapshot (fixed cells only, no movable
    // part placed) so a half-finished board can't confuse the search.
    auto solution = Solver::solve(initialBoard, initialParts);
    if (!solution) {
        statusMessage = "No solution found.";
        return;
    }

    // Reset to the clean state, then drop every part where the solver said.
    board = initialBoard;
    parts = initialParts;
    heldPartIdx = -1;
    cursorRow = 0;
    cursorCol = TRAY_COL;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        Part& p = parts[i];
        p.location.rotate = (*solution)[i].rotate;
        // Keep the visual orientation in sync with the logical rotation: the
        // renderer lerps currentAngle toward rotateCount * 90°.
        p.rotateCount = static_cast<unsigned>((*solution)[i].rotate);
        board.place(p, (*solution)[i].row, (*solution)[i].col);
    }
    statusMessage = "Solved!";
}
