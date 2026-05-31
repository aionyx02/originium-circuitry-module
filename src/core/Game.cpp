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
    // New level — drop any cached solution and hide hints.
    solutionComputed_ = false;
    solutionExists_   = false;
    solution_.clear();
    hintsVisible = false;
}

void Game::resetToInitial() {
    board = initialBoard;
    parts = initialParts;
    cursorRow = 0;
    cursorCol = TRAY_COL;
    heldPartIdx = -1;
    won = false;
    mouseControlling = false;
    hintsVisible = false;  // solution cache stays valid (same initial level)
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

void Game::ensureSolution() {
    if (solutionComputed_) return;
    solutionComputed_ = true;
    // Always solve from the clean initial snapshot (fixed cells only, no movable
    // part placed) so a half-finished board can't confuse the search.
    auto solution = Solver::solve(initialBoard, initialParts);
    if (solution) {
        solution_ = std::move(*solution);
        solutionExists_ = true;
    } else {
        solutionExists_ = false;
    }
}

void Game::setHintsVisible(bool visible) {
    if (visible) {
        ensureSolution();
        hintsVisible = solutionExists_;
    } else {
        hintsVisible = false;
    }
}

std::vector<std::pair<int, int>> Game::hintCells() const {
    std::vector<std::pair<int, int>> cells;
    if (!hintsVisible || !solutionExists_) return cells;

    for (std::size_t i = 0; i < parts.size() && i < solution_.size(); ++i) {
        Part probe = parts[i];          // shape is fixed; override placement
        probe.location = solution_[i];
        for (auto [dr, dc] : probe.rotatedCells()) {
            const int rr = solution_[i].row + dr;
            const int cc = solution_[i].col + dc;
            if (!board.inBounds(rr, cc)) continue;
            if (board._boardInfo[rr][cc] == Board::EMPTY) cells.emplace_back(rr, cc);
        }
    }
    return cells;
}

void Game::solveAndApply() {
    ensureSolution();
    if (!solutionExists_) {
        statusMessage = "No solution found.";
        return;
    }

    // Reset to the clean state, then drop every part where the solver said.
    board = initialBoard;
    parts = initialParts;
    heldPartIdx = -1;
    cursorRow = 0;
    cursorCol = TRAY_COL;
    hintsVisible = false;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        Part& p = parts[i];
        p.location.rotate = solution_[i].rotate;
        // Keep the visual orientation in sync with the logical rotation: the
        // renderer lerps currentAngle toward rotateCount * 90°.
        p.rotateCount = static_cast<unsigned>(solution_[i].rotate);
        board.place(p, solution_[i].row, solution_[i].col);
    }
    statusMessage = "Solved!";
}
