#ifndef ORIGINIUM_UI_RENDERER_H
#define ORIGINIUM_UI_RENDERER_H

#include "core/Game.h"
#include "raylib.h"

struct Layout {
    int rows;
    int cols;
    int cellSize;
    int boardX;
    int boardY;
    int trayX;
    int trayY;
};

constexpr int kTraySlotHeight = 88;
constexpr int kTraySlotWidth  = 238;

Layout computeLayout(const Game& g, int screenW, int screenH);

class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(Game& g, int screenW, int screenH, float dt);

private:
    Font uiFont{};
    bool hasUiFont = false;
};

#endif
