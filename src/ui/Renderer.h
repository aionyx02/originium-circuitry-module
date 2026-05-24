#ifndef ORIGINIUM_UI_RENDERER_H
#define ORIGINIUM_UI_RENDERER_H

#include "core/Game.h"

struct Layout {
    int rows;
    int cols;
    int cellSize;
    int boardX;
    int boardY;
    int trayX;
    int trayY;
};

constexpr int kTraySlotHeight = 78;
constexpr int kTraySlotWidth  = 200;

Layout computeLayout(const Game& g, int screenW, int screenH);

class Renderer {
public:
    void draw(Game& g, int screenW, int screenH, float dt);
};

#endif
