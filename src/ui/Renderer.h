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
    int trayY;        // first tray slot's Y, already shifted by the scroll offset
    int trayScroll;   // current vertical scroll offset of the tray, in pixels
    int trayViewTop;  // top Y of the scrollable tray viewport (clip region)
    int trayViewH;    // height of the tray viewport
    int trayViewX;    // left X of the tray viewport (clip region)
    int trayViewW;    // width of the tray viewport
};

constexpr int kTraySlotHeight = 88;
constexpr int kTraySlotWidth  = 238;

// Scrollable tray viewport. With many parts the slot list is taller than the
// screen, so it scrolls with the mouse wheel inside a fixed viewport.
constexpr int kTrayViewTop      = 118;  // top Y of the first slot at scroll 0
constexpr int kTrayBottomMargin = 84;   // gap from viewport bottom to screen bottom
constexpr int kTrayScrollSpeed  = 56;   // px scrolled per mouse-wheel notch
constexpr int kTrayViewX        = 18;   // viewport left (clip)
constexpr int kTrayViewW        = 290;  // viewport width (clip)
constexpr int kQuickButtonWidth = 118;
constexpr int kQuickButtonHeight = 30;
constexpr int kQuickButtonTop = 16;
constexpr int kQuickButtonGap = 6;
constexpr int kQuickButtonRightMargin = 16;
constexpr int kQuickButtonBottom =
    kQuickButtonTop + kQuickButtonHeight * 2 + kQuickButtonGap;

Layout computeLayout(const Game& g, int screenW, int screenH, int trayScroll = 0);

class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(Game& g, int screenW, int screenH, float dt, int trayScroll = 0);

private:
    Font uiFont{};
    bool hasUiFont = false;
    float winAnim = 0.0f;  // 0→1 fade/scale-in progress for the win banner

    // Part material skins, cycled live with the T key (wraps back to 0):
    //   0 = procedural   : original sheen band + bevel, no texture
    //   1 = carbon       : part colour + carbon weave + coloured neon glow
    //   2 = carbon dark  : near-black carbon body, colour shows only in the glow
    //   3 = stone dark   : same dark body + glow, but the tex2.png stone texture
    // Skins 1/2 need assets/tex.png, skin 3 needs assets/tex2.png; missing
    // textures are dropped from the cycle.
    Texture2D partTex{};
    Texture2D partTex2{};
    bool hasPartTex  = false;
    bool hasPartTex2 = false;
    int  partSkin    = 0;
};

#endif
