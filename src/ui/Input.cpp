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
