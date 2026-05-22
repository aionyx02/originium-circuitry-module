#include "Renderer.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

#include "raylib.h"

namespace {

constexpr int kTraySlotHeight = 78;
constexpr int kTraySlotWidth  = 200;
constexpr int kTrayInnerX     = 130;  // offset within slot where part center sits
constexpr int kTrayInnerY     = 30;

struct Layout {
    int rows;
    int cols;
    int cellSize;
    int boardX;
    int boardY;
    int trayX;
    int trayY;
};

Color partColor(unsigned partIndex) {
    static const Color palette[] = {
        { 76, 175,  80, 255},
        { 33, 150, 243, 255},
        {244,  67,  54, 255},
        {255, 193,   7, 255},
        {156,  39, 176, 255},
        {  0, 188, 212, 255},
        {255,  87,  34, 255},
        {139, 195,  74, 255},
    };
    return palette[partIndex % (sizeof(palette) / sizeof(palette[0]))];
}

Color colorBadge(unsigned colorIndex) {
    static const Color colors[] = {
        { 76, 175,  80, 255},
        { 33, 150, 243, 255},
        {244,  67,  54, 255},
        {255, 193,   7, 255},
    };
    return colors[colorIndex % (sizeof(colors) / sizeof(colors[0]))];
}

Layout computeLayout(const Game& g, int screenW, int screenH) {
    Layout L;
    L.rows = std::max(1, static_cast<int>(g.board.rows));
    L.cols = std::max(1, static_cast<int>(g.board.cols));

    const int leftReserve   = 260;
    const int rightPadding  = 60;
    const int topReserve    = 80;
    const int bottomReserve = 120;

    const int availW = screenW - leftReserve - rightPadding - 60;
    const int availH = screenH - topReserve - bottomReserve;
    int cs = std::min(availW / L.cols, availH / L.rows);
    cs = std::max(32, std::min(cs, 96));
    L.cellSize = cs;

    const int boardW = L.cols * cs;
    const int boardH = L.rows * cs;
    L.boardX = screenW - boardW - rightPadding;
    L.boardY = topReserve + (availH - boardH) / 2;

    L.trayX = 40;
    L.trayY = 90;
    return L;
}

void drawFilledBordered(int x, int y, int size, Color fill, Color border) {
    DrawRectangle(x, y, size, size, fill);
    DrawRectangleLines(x, y, size, size, border);
}

void drawCenteredText(const char* msg, int boxX, int boxY, int boxW, int boxH, int fontSize, Color tint) {
    const int tw = MeasureText(msg, fontSize);
    DrawText(msg, boxX + (boxW - tw) / 2, boxY + (boxH - fontSize) / 2, fontSize, tint);
}

void drawBoardBg(const Game& g, const Layout& L) {
    const Board& b = g.board;
    const Color emptyFill   = { 45,  50,  62, 255};
    const Color emptyBorder = { 80,  90, 110, 255};
    const Color blockedFill = { 28,  30,  38, 255};
    const Color blockedRed  = {200,  80,  80, 255};

    for (int r = 0; r < L.rows; ++r) {
        for (int c = 0; c < L.cols; ++c) {
            const int x = L.boardX + c * L.cellSize;
            const int y = L.boardY + r * L.cellSize;
            const int cell = b._boardInfo[r][c];

            if (cell == Board::CANNOT_PLACE) {
                drawFilledBordered(x, y, L.cellSize, blockedFill, blockedRed);
                drawCenteredText("X", x, y, L.cellSize, L.cellSize, 24, blockedRed);
            } else if (Board::isCannotMove(cell)) {
                const int colorIdx = Board::cannotMoveColor(cell);
                const Color cc = colorBadge(static_cast<unsigned>(colorIdx));
                const Color dim = { static_cast<unsigned char>(cc.r / 3),
                                    static_cast<unsigned char>(cc.g / 3),
                                    static_cast<unsigned char>(cc.b / 3), 255};
                drawFilledBordered(x, y, L.cellSize, dim, cc);
                drawCenteredText("=", x, y, L.cellSize, L.cellSize, 24, cc);
            } else {
                // EMPTY or OCCUPIED — empty bg under (placed parts render on top)
                drawFilledBordered(x, y, L.cellSize, emptyFill, emptyBorder);
            }
        }
    }
}

void drawConstraints(const Game& g, const Layout& L) {
    const Board& b = g.board;
    if (b.colors == 0) return;

    const int color = 0; // Phase 1: single color
    const Color tint = colorBadge(static_cast<unsigned>(color));

    for (int c = 0; c < L.cols; ++c) {
        const int x = L.boardX + c * L.cellSize;
        const int y = L.boardY - 30;
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%u", b._constraints[color][L.rows + c]);
        drawCenteredText(buf, x, y, L.cellSize, 20, 18, tint);
    }
    for (int r = 0; r < L.rows; ++r) {
        const int x = L.boardX - 30;
        const int y = L.boardY + r * L.cellSize;
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%u", b._constraints[color][r]);
        drawCenteredText(buf, x, y, 24, L.cellSize, 18, tint);
    }
}

void drawPlacementHighlight(const Game& g, const Layout& L) {
    if (g.heldPartIdx < 0) return;
    if (g.cursorCol < 0) return;
    const Part& p = g.parts[g.heldPartIdx];
    // location.row/col = bbox top-left (synced from cursor via pivot offset)
    const bool valid = g.board.canPlace(p, p.location.row, p.location.col);
    Color fill    = valid ? Color{ 80, 220, 100,  70 } : Color{ 220, 80, 80,  80 };
    Color outline = valid ? Color{ 80, 220, 100, 200 } : Color{ 220, 80, 80, 200 };

    for (auto [dr, dc] : p.rotatedCells()) {
        const int rr = p.location.row + dr;
        const int cc = p.location.col + dc;
        if (rr < 0 || cc < 0 ||
            rr >= L.rows || cc >= L.cols) continue;
        const int x = L.boardX + cc * L.cellSize;
        const int y = L.boardY + rr * L.cellSize;
        DrawRectangle(x, y, L.cellSize, L.cellSize, fill);
        DrawRectangleLinesEx(Rectangle{(float)x, (float)y,
                                       (float)L.cellSize, (float)L.cellSize}, 2, outline);
    }
}

void drawCursor(const Game& g, const Layout& L) {
    if (g.cursorCol >= 0) {
        const int x = L.boardX + g.cursorCol * L.cellSize;
        const int y = L.boardY + g.cursorRow * L.cellSize;
        DrawRectangleLinesEx(Rectangle{(float)x - 2, (float)y - 2,
                                       (float)L.cellSize + 4, (float)L.cellSize + 4}, 3, YELLOW);
    } else {
        const int y = L.trayY + g.cursorRow * kTraySlotHeight - 4;
        const int x = L.trayX - 6;
        DrawRectangleLinesEx(Rectangle{(float)x - 2, (float)y - 2,
                                       (float)kTraySlotWidth + 4, (float)kTraySlotHeight + 4}, 3, YELLOW);
    }
}

void drawTrayBg(const Game& g, const Layout& L) {
    DrawText("TRAY", L.trayX, L.trayY - 30, 18, RAYWHITE);
    for (size_t i = 0; i < g.parts.size(); ++i) {
        const int slotY = L.trayY + static_cast<int>(i) * kTraySlotHeight;
        const bool placed = g.parts[i].location.placed;
        const bool held   = static_cast<int>(i) == g.heldPartIdx;
        const bool inTray = !placed && !held;

        const Color slotBg = inTray ? Color{45, 50, 62, 255} : Color{35, 38, 46, 255};
        DrawRectangle(L.trayX - 6, slotY - 6, kTraySlotWidth, kTraySlotHeight, slotBg);
        DrawRectangleLines(L.trayX - 6, slotY - 6, kTraySlotWidth, kTraySlotHeight, Color{90, 100, 120, 255});

        char label[32];
        std::snprintf(label, sizeof(label), "P%zu  c%u", i, g.parts[i].colorIndex);
        DrawText(label, L.trayX, slotY - 2, 14, RAYWHITE);

        const char* state =
            placed ? "placed" :
            held   ? "held"   : "in tray";
        DrawText(state, L.trayX, slotY + kTraySlotHeight - 24, 12, Color{180, 185, 200, 255});
    }
}

void drawStatus(const Game& g, int screenW, int screenH) {
    if (!g.statusMessage.empty()) {
        DrawText(g.statusMessage.c_str(), 40, screenH - 80, 18, Color{255, 200, 100, 255});
    }
    DrawText("WASD/Arrows: move   R: rotate   Enter/Space: place/pick   Esc/Bksp: drop/remove",
             40, screenH - 40, 16, Color{160, 170, 190, 255});
    (void)screenW;
}

void drawWinBanner(int screenW, int screenH) {
    const int bw = 380, bh = 90;
    const int bx = (screenW - bw) / 2;
    const int by = (screenH - bh) / 2;
    DrawRectangle(bx, by, bw, bh, Color{0, 0, 0, 210});
    DrawRectangleLinesEx(Rectangle{(float)bx, (float)by, (float)bw, (float)bh}, 3, GOLD);
    drawCenteredText("You Win!", bx, by, bw, bh, 40, GOLD);
}

// --- Phase 2 animation ---

void computeTarget(const Part& p, int partIdx, const Game& g, const Layout& L,
                   float& tx, float& ty, float& tAngle, float& tScale) {
    const bool held = (partIdx == g.heldPartIdx);
    // Monotonic CW angle — no shortest-arc wrap needed.
    tAngle = static_cast<float>(p.rotateCount) * 90.0f;

    // Center cell position within rotated shape (where the pivot cell ends up
    // after rotation). The pivot is the part's logical rotation/move center.
    int rCr = 0, rCc = 0;
    p.rotatedCenterCell(rCr, rCc);

    if (p.location.placed) {
        tx = L.boardX + (p.location.col + rCc + 0.5f) * L.cellSize;
        ty = L.boardY + (p.location.row + rCr + 0.5f) * L.cellSize;
        tScale = 1.0f;
    } else if (held) {
        if (g.cursorCol == Game::TRAY_COL) {
            tx = static_cast<float>(L.trayX - 6 + kTrayInnerX);
            ty = static_cast<float>(L.trayY + g.cursorRow * kTraySlotHeight + kTrayInnerY);
            tScale = 0.5f;
        } else {
            // Cursor IS the pivot target — pivot stays put through rotation.
            tx = L.boardX + (g.cursorCol + 0.5f) * L.cellSize;
            ty = L.boardY + (g.cursorRow + 0.5f) * L.cellSize;
            tScale = 1.0f;
            (void)rCr; (void)rCc;
        }
    } else {
        tx = static_cast<float>(L.trayX - 6 + kTrayInnerX);
        ty = static_cast<float>(L.trayY + partIdx * kTraySlotHeight + kTrayInnerY);
        tScale = 0.4f;
    }
}

void animateParts(Game& g, const Layout& L, float dt) {
    const float posFactor   = 1.0f - std::exp(-dt * 14.0f);
    const float angleFactor = 1.0f - std::exp(-dt * 12.0f);
    const float scaleFactor = 1.0f - std::exp(-dt * 12.0f);

    for (size_t i = 0; i < g.parts.size(); ++i) {
        Part& p = g.parts[i];
        float tx, ty, tAngle, tScale;
        computeTarget(p, static_cast<int>(i), g, L, tx, ty, tAngle, tScale);
        if (!p.visualInitialized) {
            p.currentCenterX = tx;
            p.currentCenterY = ty;
            p.currentAngle   = tAngle;
            p.currentScale   = tScale;
            p.visualInitialized = true;
        } else {
            p.currentCenterX += (tx - p.currentCenterX) * posFactor;
            p.currentCenterY += (ty - p.currentCenterY) * posFactor;
            p.currentAngle   += (tAngle - p.currentAngle) * angleFactor;
            p.currentScale   += (tScale - p.currentScale) * scaleFactor;
        }
    }
}

void drawPartAnimated(const Part& p, float cellSize, Color fill, float alpha) {
    const int M = static_cast<int>(p.shape.size());
    const int N = M > 0 ? static_cast<int>(p.shape[0].size()) : 0;
    if (M == 0 || N == 0) return;
    const float scs = cellSize * p.currentScale;

    Color baseFill = fill;
    baseFill.a = static_cast<unsigned char>(255 * alpha);
    // Center cell uses brighter shade so the pivot is obvious. Blend toward
    // white by +120 per channel; stays distinct across any part color.
    Color centerFill = Color{
        static_cast<unsigned char>(std::min(255, fill.r + 120)),
        static_cast<unsigned char>(std::min(255, fill.g + 120)),
        static_cast<unsigned char>(std::min(255, fill.b + 120)),
        static_cast<unsigned char>(255 * alpha)
    };

    // Pivot = un-rotated center cell's center. Each cell drawn with origin offset
    // = (centerCol - col + 0.5, centerRow - row + 0.5) * scs so DrawRectanglePro
    // rotates all cells around the pivot.
    const float cc = static_cast<float>(p.centerCellCol);
    const float cr = static_cast<float>(p.centerCellRow);
    for (int r = 0; r < M; ++r) {
        for (int col = 0; col < N; ++col) {
            if (!p.shape[r][col]) continue;
            const bool isCenter = (r == p.centerCellRow && col == p.centerCellCol);
            Rectangle rec = { p.currentCenterX, p.currentCenterY, scs, scs };
            Vector2 origin = { (cc - col + 0.5f) * scs,
                               (cr - r   + 0.5f) * scs };
            DrawRectanglePro(rec, origin, p.currentAngle,
                             isCenter ? centerFill : baseFill);
        }
    }
}

void drawParts(const Game& g, const Layout& L) {
    for (size_t i = 0; i < g.parts.size(); ++i) {
        const Part& p = g.parts[i];
        const bool held = (static_cast<int>(i) == g.heldPartIdx);
        const bool inTray = !p.location.placed && !held;
        const Color fill = partColor(p.partIndex);
        const float alpha = inTray ? 0.85f : 1.0f;
        drawPartAnimated(p, static_cast<float>(L.cellSize), fill, alpha);
    }
}

} // namespace

void Renderer::draw(Game& g, int screenW, int screenH, float dt) {
    ClearBackground(Color{25, 28, 36, 255});
    DrawText("Originium Circuit Repair", 40, 24, 24, RAYWHITE);

    const Layout L = computeLayout(g, screenW, screenH);

    animateParts(g, L, dt);

    drawConstraints(g, L);
    drawBoardBg(g, L);
    drawPlacementHighlight(g, L);
    drawCursor(g, L);
    drawTrayBg(g, L);
    drawParts(g, L);
    drawStatus(g, screenW, screenH);

    if (g.won) drawWinBanner(screenW, screenH);
}
