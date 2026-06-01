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
constexpr int kQuickButtonWidth = 136;
constexpr int kQuickButtonHeight = 34;
constexpr int kQuickButtonTop = 16;
constexpr int kQuickButtonGap = 6;
constexpr int kQuickButtonRightMargin = 16;
constexpr int kQuickButtonBottom =
    kQuickButtonTop + kQuickButtonHeight * 2 + kQuickButtonGap;

Layout computeLayout(const Game& g, int screenW, int screenH);

class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(Game& g, int screenW, int screenH, float dt);

private:
    Font uiFont{};
    bool hasUiFont = false;
    float winAnim = 0.0f;  // 0→1 fade/scale-in progress for the win banner
};

#endif
