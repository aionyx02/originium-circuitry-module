#ifndef ORIGINIUM_UI_RENDERER_H
#define ORIGINIUM_UI_RENDERER_H

#include "core/Game.h"

class Renderer {
public:
    void draw(Game& g, int screenW, int screenH, float dt);
};

#endif
