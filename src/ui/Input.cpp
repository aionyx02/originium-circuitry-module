#include "Input.h"

#include "raylib.h"

Action Input::poll() {
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    return Action::MoveUp;
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  return Action::MoveDown;
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  return Action::MoveLeft;
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) return Action::MoveRight;
    if (IsKeyPressed(KEY_R))                            return Action::Rotate;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) return Action::Place;
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) return Action::Remove;
    return Action::None;
}

void Input::pollMouse(const Layout& L, Game& g) {
    if (g.won) return;

    const Vector2 m = GetMousePosition();
    const Vector2 d = GetMouseDelta();
    const bool mouseMoved = (d.x != 0.0f) || (d.y != 0.0f);
    const bool leftClick  = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    const bool rightClick = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
    const bool mouseActive = mouseMoved || leftClick || rightClick;

    // Board hit-test
    int boardRow = -1, boardCol = -1;
    {
        const float bx = m.x - L.boardX;
        const float by = m.y - L.boardY;
        if (bx >= 0 && by >= 0) {
            const int c = static_cast<int>(bx) / L.cellSize;
            const int r = static_cast<int>(by) / L.cellSize;
            if (c >= 0 && c < L.cols && r >= 0 && r < L.rows) {
                boardRow = r;
                boardCol = c;
            }
        }
    }

    // Tray hit-test (mouse can't be over both — tray is left, board is right)
    int trayRow = -1;
    if (boardRow < 0) {
        const int slotX0 = L.trayX - 6;
        const int slotX1 = slotX0 + kTraySlotWidth;
        if (m.x >= slotX0 && m.x < slotX1) {
            const int relY = static_cast<int>(m.y) - L.trayY;
            if (relY >= 0) {
                const int slot = relY / kTraySlotHeight;
                if (slot >= 0 && slot < static_cast<int>(g.parts.size())) {
                    trayRow = slot;
                }
            }
        }
    }

    // Any mouse activity → enter / stay in mouse mode (cleared by keyboard Move*).
    if (mouseActive) g.mouseControlling = true;

    if (leftClick) {
        if (boardRow >= 0) {
            // Empty cell + held → place. Occupied cell + not held → pick up.
            // Cursor (cell-quantized) is set to the click cell so handlePlace
            // uses the red/green preview cell as the placement target.
            g.setCursor(boardRow, boardCol, false);
            g.update(Action::Place);
        } else if (trayRow >= 0 && g.heldPartIdx < 0) {
            // Pick up from tray (only when hands are empty).
            g.setCursor(trayRow, 0, true);
            g.update(Action::Place);
        }
    }

    if (rightClick) {
        // Cancel held → back to tray. On board occupied cell w/o held → remove.
        if (boardRow >= 0) g.setCursor(boardRow, boardCol, false);
        g.update(Action::Remove);
    }

    // Drag follow (cursor = preview cell, visual snap handled in Renderer):
    // keep cursor under the mouse every frame while in mouse mode + holding +
    // hovering the board. Runs after click handlers so that the auto-selected
    // next part also immediately follows the mouse on the same frame as a place.
    if (g.mouseControlling && g.heldPartIdx >= 0 && boardRow >= 0) {
        g.setCursor(boardRow, boardCol, false);
    }
}
