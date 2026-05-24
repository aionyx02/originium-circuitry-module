#ifndef ORIGINIUM_UI_INPUT_H
#define ORIGINIUM_UI_INPUT_H

#include "core/Game.h"
#include "ui/Renderer.h"  // Layout

class Input {
public:
    static Action poll();

    // Mouse: hit-test against board & tray using current layout, then map
    // left/right click to the existing Action::Place / Action::Remove flow.
    // Cursor follows the mouse only while a part is held (drag follow).
    static void pollMouse(const Layout& L, Game& g);
};

#endif
