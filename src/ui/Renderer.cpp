#include "Renderer.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

#include "raylib.h"
#include "ui/Theme.h"

namespace {

using theme::colorBadge;

constexpr int kTrayPreviewX   = 124;  // offset within slot where preview box starts
constexpr int kTrayPreviewW   = 98;
constexpr int kConstraintGap  = 8;
constexpr int kColHintHeight  = 48;
constexpr int kRowHintWidth   = kColHintHeight;

Color hintStatusColor(unsigned current, unsigned need) {
    if (current == need) return Color{80, 220, 100, 255};
    return Color{174, 191, 203, 255};
}

} // namespace

Layout computeLayout(const Game& g, int screenW, int screenH) {
    Layout L;
    L.rows = std::max(1, static_cast<int>(g.board.rows));
    L.cols = std::max(1, static_cast<int>(g.board.cols));

    const int leftReserve   = 330;
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

    L.trayX = 46;
    L.trayY = 118;
    return L;
}

namespace {

void drawRoundedCell(int x, int y, int size, Color fill, Color border) {
    Rectangle r = { (float)x, (float)y, (float)size, (float)size };
    DrawRectangleRounded(r, 0.18f, 6, fill);
    DrawRectangleRoundedLinesEx(r, 0.18f, 6, 1.0f, border);
}

void drawRoundedRect(Rectangle r, float roundness, Color fill, Color border, float borderThick) {
    DrawRectangleRounded(r, roundness, 8, fill);
    DrawRectangleRoundedLinesEx(r, roundness, 8, borderThick, border);
}

void drawGlowRect(Rectangle r, float roundness, Color color) {
    Color soft = color;
    soft.a = 42;
    DrawRectangleRounded(Rectangle{r.x - 3.0f, r.y - 3.0f, r.width + 6.0f, r.height + 6.0f},
                         roundness, 8, soft);
    DrawRectangleRoundedLinesEx(r, roundness, 8, 2.0f, color);
}

float textWidth(Font font, const char* msg, float fontSize, float spacing = 1.0f) {
    return MeasureTextEx(font, msg, fontSize, spacing).x;
}

void drawText(Font font, const char* msg, float x, float y, float fontSize, Color tint,
              float spacing = 1.0f) {
    DrawTextEx(font, msg, Vector2{x, y}, fontSize, spacing, tint);
}

void drawCenteredText(Font font, const char* msg, int boxX, int boxY, int boxW, int boxH,
                      int fontSize, Color tint) {
    const Vector2 size = MeasureTextEx(font, msg, static_cast<float>(fontSize), 1.0f);
    DrawTextEx(font, msg,
               Vector2{boxX + (boxW - size.x) / 2.0f,
                       boxY + (boxH - size.y) / 2.0f},
               static_cast<float>(fontSize), 1.0f, tint);
}

void drawBarSegment(Rectangle r, Color fill, Color border, bool active) {
    DrawRectangleRounded(r, 0.38f, 5, fill);
    DrawRectangleRoundedLinesEx(r, 0.38f, 5, active ? 1.0f : 1.5f, border);
}

void drawConstraintBars(unsigned current, unsigned need,
                        int boxX, int boxY, int boxW, int boxH,
                        Color status, Color colorTag, bool rowHint) {
    const unsigned slots = std::max(need, current);
    if (slots == 0) return;

    const bool satisfied = current == need;
    const Color overflow = Color{220, 80, 80, 255};
    const Color active = satisfied ? status : colorTag;
    Color inactiveBorder = colorTag;
    inactiveBorder.a = 150;

    if (rowHint) {
        const int gap = 3;
        const int totalGaps = gap * (static_cast<int>(slots) - 1);
        const int barW = std::max(2, std::min(6, (boxW - 8 - totalGaps) / static_cast<int>(slots)));
        const int barH = std::max(8, std::min(18, boxH / 2));
        const int groupW = static_cast<int>(slots) * barW + totalGaps;
        const int rightPad = groupW + 4 <= boxW ? 4 : std::max(0, (boxW - groupW) / 2);
        const int startX = boxX + boxW - groupW - rightPad;
        const int y = boxY + (boxH - barH) / 2;
        for (unsigned i = 0; i < slots; ++i) {
            const unsigned logicalIdx = slots - 1 - i;
            const bool filledSlot = logicalIdx < current;
            const bool overflowSlot = logicalIdx >= need && logicalIdx < current;
            const Color fill = overflowSlot ? overflow :
                               filledSlot   ? active :
                               Color{0, 0, 0, 0};
            const Color border = overflowSlot ? overflow :
                                 filledSlot   ? active : inactiveBorder;
            const int x = startX + static_cast<int>(i) * (barW + gap);
            drawBarSegment(Rectangle{(float)x, (float)y, (float)barW, (float)barH},
                           fill, border, filledSlot);
        }
    } else {
        const int gap = 3;
        const int totalGaps = gap * (static_cast<int>(slots) - 1);
        const int barH = std::max(2, std::min(5, (boxH - 8 - totalGaps) / static_cast<int>(slots)));
        const int barW = std::max(16, std::min(30, boxW - 8));
        const int groupH = static_cast<int>(slots) * barH + totalGaps;
        const int x = boxX + (boxW - barW) / 2;
        const int startY = boxY + boxH - groupH - 4;
        for (unsigned i = 0; i < slots; ++i) {
            const bool filledSlot = i < current;
            const bool overflowSlot = i >= need && i < current;
            const Color fill = overflowSlot ? overflow :
                               filledSlot   ? active :
                               Color{0, 0, 0, 0};
            const Color border = overflowSlot ? overflow :
                                 filledSlot   ? active : inactiveBorder;
            const int y = startY + static_cast<int>(slots - 1 - i) * (barH + gap);
            drawBarSegment(Rectangle{(float)x, (float)y, (float)barW, (float)barH},
                           fill, border, filledSlot);
        }
    }
}

void drawBoardBg(const Game& g, const Layout& L, Font font) {
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
                drawRoundedCell(x, y, L.cellSize, blockedFill, blockedRed);
                drawCenteredText(font, "X", x, y, L.cellSize, L.cellSize, 24, blockedRed);
            } else if (Board::isCannotMove(cell)) {
                const int colorIdx = Board::cannotMoveColor(cell);
                const Color cc = colorBadge(static_cast<unsigned>(colorIdx));
                const Color dim = { static_cast<unsigned char>(cc.r / 3),
                                    static_cast<unsigned char>(cc.g / 3),
                                    static_cast<unsigned char>(cc.b / 3), 255};
                drawRoundedCell(x, y, L.cellSize, dim, cc);
                drawCenteredText(font, "=", x, y, L.cellSize, L.cellSize, 24, cc);
            } else {
                // EMPTY or OCCUPIED — empty bg under (placed parts render on top)
                drawRoundedCell(x, y, L.cellSize, emptyFill, emptyBorder);
            }
        }
    }
}

void drawConstraints(const Game& g, const Layout& L, Font font) {
    const Board& b = g.board;
    (void)font;
    if (b.colors == 0) return;

    const int colorCount = static_cast<int>(b.colors);
    const int laneCount = colorCount > 1 ? std::max(2, colorCount) : 1;
    for (int color = 0; color < colorCount; ++color) {
        const Color tint = colorBadge(static_cast<unsigned>(color));

        for (int c = 0; c < L.cols; ++c) {
            const int laneX0 = L.boardX + c * L.cellSize + (color * L.cellSize) / laneCount;
            const int laneX1 = L.boardX + c * L.cellSize + ((color + 1) * L.cellSize) / laneCount;
            const int x = laneX0;
            const int y = L.boardY - kConstraintGap - kColHintHeight;
            const int w = std::max(8, laneX1 - laneX0);
            const unsigned need = b._constraints[color][L.rows + c];
            const unsigned current = b.currentFilledForColor(color, c, false, g.parts);
            const Color status = hintStatusColor(current, need);
            drawConstraintBars(current, need, x, y, w, kColHintHeight,
                               status, tint, false);
        }
        for (int r = 0; r < L.rows; ++r) {
            const int rowHintX = L.boardX - kConstraintGap - kRowHintWidth;
            const int rowLane = colorCount > 1 ? (colorCount - 1 - color) : color;
            const int laneY0 = L.boardY + r * L.cellSize + (rowLane * L.cellSize) / laneCount;
            const int laneY1 = L.boardY + r * L.cellSize + ((rowLane + 1) * L.cellSize) / laneCount;
            const int x = rowHintX;
            const int y = laneY0;
            const int h = std::max(8, laneY1 - laneY0);
            const unsigned need = b._constraints[color][r];
            const unsigned current = b.currentFilledForColor(color, r, true, g.parts);
            const Color status = hintStatusColor(current, need);
            drawConstraintBars(current, need, x, y, kRowHintWidth, h,
                               status, tint, true);
        }
    }
}

void drawHints(const Game& g, const Layout& L) {
    if (!g.hintsVisible) return;
    const Color fill   = Color{ 80, 220, 100,  70 };
    const Color border = Color{ 80, 220, 100, 150 };
    for (auto [r, c] : g.hintCells()) {
        if (r < 0 || c < 0 || r >= L.rows || c >= L.cols) continue;
        const int x = L.boardX + c * L.cellSize;
        const int y = L.boardY + r * L.cellSize;
        Rectangle rec = { (float)x, (float)y, (float)L.cellSize, (float)L.cellSize };
        DrawRectangleRounded(rec, 0.18f, 6, fill);
        DrawRectangleRoundedLinesEx(rec, 0.18f, 6, 1.5f, border);
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
    const Color cursor = theme::kAccent;
    if (g.cursorCol >= 0) {
        const int x = L.boardX + g.cursorCol * L.cellSize;
        const int y = L.boardY + g.cursorRow * L.cellSize;
        DrawRectangleLinesEx(Rectangle{(float)x - 2, (float)y - 2,
                                       (float)L.cellSize + 4, (float)L.cellSize + 4}, 3, cursor);
    } else {
        const int y = L.trayY + g.cursorRow * kTraySlotHeight - 4;
        const int x = L.trayX - 6;
        drawGlowRect(Rectangle{(float)x - 1.0f, (float)y - 1.0f,
                               (float)kTraySlotWidth + 2.0f, (float)kTraySlotHeight + 2.0f},
                     0.10f, cursor);
    }
}

void drawSidebar(const Game& g, const Layout& L, Font font) {
    const int partCount = static_cast<int>(g.parts.size());
    const float panelH = std::max(430.0f, partCount * kTraySlotHeight + 140.0f);
    Rectangle panel = {24.0f, 20.0f, 276.0f, panelH};
    drawRoundedRect(panel, 0.08f, Color{18, 22, 32, 224}, theme::kPanelBorder, 1.0f);
    DrawRectangleGradientV(25, 21, 274, static_cast<int>(panelH) - 2,
                           Color{35, 43, 56, 120}, Color{16, 20, 30, 40});

    drawText(font, "ORIGINIUM", 44.0f, 30.0f, 27.0f, theme::kTextBright, 1.5f);
    drawText(font, "CIRCUIT REPAIR", 46.0f, 59.0f, 14.0f, theme::kAccent, 1.2f);

    DrawRectangle(46, 90, 70, 2, Color{109, 236, 218, 190});
    drawText(font, "PARTS CACHE", static_cast<float>(L.trayX), static_cast<float>(L.trayY - 30),
             15.0f, theme::kTextMuted, 1.4f);
}

void drawTrayBg(const Game& g, const Layout& L, Font font) {
    for (size_t i = 0; i < g.parts.size(); ++i) {
        const int slotY = L.trayY + static_cast<int>(i) * kTraySlotHeight;
        const bool placed = g.parts[i].location.placed;
        const bool held   = static_cast<int>(i) == g.heldPartIdx;
        const bool inTray = !placed && !held;

        const Color slotBg =
            held   ? Color{36, 55, 61, 255} :
            inTray ? Color{31, 37, 48, 255} :
                     Color{21, 25, 33, 230};
        const Color border =
            held   ? Color{109, 236, 218, 255} :
            placed ? Color{64, 79, 88, 210} :
                     Color{75, 88, 104, 255};
        Rectangle slotRec = { (float)(L.trayX - 6), (float)(slotY - 6),
                              (float)kTraySlotWidth, (float)kTraySlotHeight };
        drawRoundedRect(slotRec, 0.10f, slotBg, border, held ? 2.0f : 1.0f);

        char label[32];
        std::snprintf(label, sizeof(label), "PART %02zu", i + 1);
        drawText(font, label, static_cast<float>(L.trayX), static_cast<float>(slotY),
                 15.0f, placed ? Color{114, 128, 139, 255} : Color{232, 244, 245, 255}, 1.0f);

        const Color cc = colorBadge(g.parts[i].colorIndex);
        DrawRectangleRounded(Rectangle{(float)L.trayX, (float)(slotY + 26), 44.0f, 8.0f},
                             0.8f, 6, placed ? Color{cc.r, cc.g, cc.b, 80} : cc);

        const Rectangle preview = {(float)(L.trayX + kTrayPreviewX), (float)(slotY + 10),
                                   (float)kTrayPreviewW, (float)(kTraySlotHeight - 22)};
        Color previewFrame = held ? Color{109, 236, 218, 86} : Color{74, 91, 105, 95};
        DrawRectangleRoundedLinesEx(preview, 0.12f, 6, 1.0f, previewFrame);

        const char* state =
            placed ? "INSTALLED" :
            held   ? "ACTIVE"    : "READY";
        const Color stateColor =
            held   ? Color{109, 236, 218, 255} :
            placed ? Color{112, 124, 132, 255} :
                     Color{170, 184, 196, 255};
        drawText(font, state, static_cast<float>(L.trayX),
                 static_cast<float>(slotY + kTraySlotHeight - 30), 13.0f, stateColor, 1.2f);
    }
}

void drawStatus(const Game& g, int screenW, int screenH, Font font) {
    if (!g.statusMessage.empty()) {
        Rectangle msgBg = {34.0f, (float)screenH - 92.0f,
                           std::min(560.0f, textWidth(font, g.statusMessage.c_str(), 18.0f) + 28.0f),
                           34.0f};
        drawRoundedRect(msgBg, 0.18f, Color{48, 33, 25, 230}, Color{255, 176, 88, 220}, 1.0f);
        drawText(font, g.statusMessage.c_str(), 48.0f, static_cast<float>(screenH - 84),
                 18.0f, Color{255, 203, 126, 255}, 1.0f);
    }
    // Faint chrome strip so the hint line reads as a footer, not floating text.
    DrawRectangle(0, screenH - 48, screenW, 48, Color{12, 16, 24, 150});
    DrawRectangle(0, screenH - 48, screenW, 1, Color{72, 94, 112, 90});
    drawText(font, "WASD MOVE  ·  R ROTATE  ·  ENTER/CLICK PLACE  ·  ESC/RMB REMOVE  ·  F SOLVE  ·  BKSP RESTART  ·  N MENU",
             40.0f, static_cast<float>(screenH - 38), 15.0f, Color{143, 160, 174, 255}, 1.0f);
}

void drawWinBanner(int screenW, int screenH, Font font, float anim) {
    // anim: 0→1 eased progress. Fade alpha in and slide the banner up a touch.
    const float a = std::clamp(anim, 0.0f, 1.0f);
    auto fade = [a](Color c) {
        c.a = static_cast<unsigned char>(c.a * a);
        return c;
    };
    const int bw = 430, bh = 132;
    const int bx = (screenW - bw) / 2;
    const int slide = static_cast<int>((1.0f - a) * 14.0f);  // starts lower, settles
    const int by = (screenH - bh) / 2 + slide;
    Rectangle r = { (float)bx, (float)by, (float)bw, (float)bh };
    drawRoundedRect(r, 0.12f, fade(Color{9, 14, 20, 232}), fade(theme::kAccent), 2.5f);
    drawCenteredText(font, "CIRCUIT RESTORED", bx, by + 18, bw, 40, 32, fade(Color{230, 249, 246, 255}));
    drawCenteredText(font, "All constraints satisfied", bx, by + 58, bw, 22, 16, fade(theme::kAccent));
    drawCenteredText(font, "N: Menu      Backspace: Replay", bx, by + 90, bw, 22, 16,
                     fade(theme::kTextMuted));
}

// --- Phase 2 animation ---

void computeRotatedBounds(const Part& p, int& minR, int& maxR, int& minC, int& maxC) {
    minR = 0;
    maxR = 0;
    minC = 0;
    maxC = 0;
    bool any = false;
    for (auto [dr, dc] : p.rotatedCells()) {
        if (!any) {
            minR = maxR = dr;
            minC = maxC = dc;
            any = true;
        } else {
            minR = std::min(minR, dr);
            maxR = std::max(maxR, dr);
            minC = std::min(minC, dc);
            maxC = std::max(maxC, dc);
        }
    }
}

void computeTrayPreviewTarget(const Part& p, int partIdx, const Layout& L,
                              float& tx, float& ty, float& tScale) {
    int minR, maxR, minC, maxC;
    computeRotatedBounds(p, minR, maxR, minC, maxC);

    int rCr = 0;
    int rCc = 0;
    p.rotatedCenterCell(rCr, rCc);

    const int shapeRows = std::max(1, maxR - minR + 1);
    const int shapeCols = std::max(1, maxC - minC + 1);
    const float previewW = static_cast<float>(kTrayPreviewW - 16);
    const float previewH = static_cast<float>(kTraySlotHeight - 30);
    tScale = std::min(previewW / (shapeCols * L.cellSize),
                      previewH / (shapeRows * L.cellSize));
    tScale = std::min(tScale, 0.48f);

    const float scs = L.cellSize * tScale;
    const float previewCenterX = L.trayX + kTrayPreviewX + kTrayPreviewW * 0.5f;
    const float previewCenterY = L.trayY + partIdx * kTraySlotHeight + kTraySlotHeight * 0.48f;
    const float occupiedCenterC = (minC + maxC + 1) * 0.5f;
    const float occupiedCenterR = (minR + maxR + 1) * 0.5f;
    tx = previewCenterX - (occupiedCenterC - rCc) * scs;
    ty = previewCenterY - (occupiedCenterR - rCr) * scs;
}

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
        if (g.mouseControlling) {
            // Pixel-perfect drag from the instant of pickup, regardless of
            // whether cursor is still on tray (just clicked) or on the board.
            // Aiming is done by the red/green preview frame at the cursor cell;
            // on release the existing lerp slides the placed part to cell center.
            const Vector2 mp = GetMousePosition();
            tx = mp.x;
            ty = mp.y;
            tScale = 1.0f;
            (void)rCr; (void)rCc;
        } else if (g.cursorCol == Game::TRAY_COL) {
            // Keyboard + held in tray: small preview at tray slot.
            computeTrayPreviewTarget(p, g.cursorRow, L, tx, ty, tScale);
        } else {
            // Keyboard + on board: cursor IS the pivot target.
            tx = L.boardX + (g.cursorCol + 0.5f) * L.cellSize;
            ty = L.boardY + (g.cursorRow + 0.5f) * L.cellSize;
            tScale = 1.0f;
            (void)rCr; (void)rCc;
        }
    } else {
        computeTrayPreviewTarget(p, partIdx, L, tx, ty, tScale);
    }
}

void animateParts(Game& g, const Layout& L, float dt) {
    const float posFactor   = 1.0f - std::exp(-dt * 14.0f);
    const float angleFactor = 1.0f - std::exp(-dt * 12.0f);
    const float scaleFactor = 1.0f - std::exp(-dt * 12.0f);

    for (size_t i = 0; i < g.parts.size(); ++i) {
        Part& p = g.parts[i];
        const bool held = (static_cast<int>(i) == g.heldPartIdx);
        // Pixel-perfect drag covers tray-held too — first frame after a click
        // pickup should already snap to mouse (don't wait for mouse to enter board).
        const bool mouseDrag = held && g.mouseControlling;

        float tx, ty, tAngle, tScale;
        computeTarget(p, static_cast<int>(i), g, L, tx, ty, tAngle, tScale);

        if (!p.visualInitialized) {
            p.currentCenterX = tx;
            p.currentCenterY = ty;
            p.currentAngle   = tAngle;
            p.currentScale   = tScale;
            p.visualInitialized = true;
        } else if (mouseDrag) {
            // Pixel-perfect: snap position to mouse (no lag); lerp angle/scale.
            p.currentCenterX = tx;
            p.currentCenterY = ty;
            p.currentAngle  += (tAngle - p.currentAngle) * angleFactor;
            p.currentScale  += (tScale - p.currentScale) * scaleFactor;
        } else {
            p.currentCenterX += (tx - p.currentCenterX) * posFactor;
            p.currentCenterY += (ty - p.currentCenterY) * posFactor;
            p.currentAngle   += (tAngle - p.currentAngle) * angleFactor;
            p.currentScale   += (tScale - p.currentScale) * scaleFactor;
        }
    }
}

void drawPartShadow(const Part& p, float cellSize, float alpha) {
    const int M = static_cast<int>(p.shape.size());
    const int N = M > 0 ? static_cast<int>(p.shape[0].size()) : 0;
    if (M == 0 || N == 0) return;
    const float scs = cellSize * p.currentScale;
    const float cc = static_cast<float>(p.centerCellCol);
    const float cr = static_cast<float>(p.centerCellRow);
    const float dx = 4.0f, dy = 5.0f;
    Color shadow = Color{0, 0, 0, static_cast<unsigned char>(110 * alpha)};
    for (int r = 0; r < M; ++r) {
        for (int col = 0; col < N; ++col) {
            if (!p.shape[r][col]) continue;
            Rectangle rec = { p.currentCenterX + dx, p.currentCenterY + dy, scs, scs };
            Vector2 origin = { (cc - col + 0.5f) * scs,
                               (cr - r   + 0.5f) * scs };
            DrawRectanglePro(rec, origin, p.currentAngle, shadow);
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
    // Center cell uses brighter shade so the pivot is obvious.
    Color centerFill = Color{
        static_cast<unsigned char>(std::min(255, fill.r + 120)),
        static_cast<unsigned char>(std::min(255, fill.g + 120)),
        static_cast<unsigned char>(std::min(255, fill.b + 120)),
        static_cast<unsigned char>(255 * alpha)
    };
    // Highlight band (top sheen) — procedural texture
    Color highlight = Color{
        static_cast<unsigned char>(std::min(255, fill.r + 70)),
        static_cast<unsigned char>(std::min(255, fill.g + 70)),
        static_cast<unsigned char>(std::min(255, fill.b + 70)),
        static_cast<unsigned char>(170 * alpha)
    };
    // Dark bottom edge — gives each cell a 3D bevel feel
    Color edge = Color{
        static_cast<unsigned char>(fill.r / 2),
        static_cast<unsigned char>(fill.g / 2),
        static_cast<unsigned char>(fill.b / 2),
        static_cast<unsigned char>(220 * alpha)
    };

    const float cc = static_cast<float>(p.centerCellCol);
    const float cr = static_cast<float>(p.centerCellRow);
    const float inset = 0.06f * scs;
    const float bandH = scs * 0.32f;
    const float edgeH = scs * 0.10f;

    for (int r = 0; r < M; ++r) {
        for (int col = 0; col < N; ++col) {
            if (!p.shape[r][col]) continue;
            const bool isCenter = (r == p.centerCellRow && col == p.centerCellCol);

            // Base cell fill
            Rectangle rec = { p.currentCenterX, p.currentCenterY, scs, scs };
            Vector2 origin = { (cc - col + 0.5f) * scs,
                               (cr - r   + 0.5f) * scs };
            DrawRectanglePro(rec, origin, p.currentAngle,
                             isCenter ? centerFill : baseFill);

            // Highlight band at top of cell (slightly inset horizontally)
            Rectangle bandRec = { p.currentCenterX, p.currentCenterY,
                                  scs - 2 * inset, bandH };
            Vector2 bandOrigin = { (cc - col + 0.5f) * scs - inset,
                                   (cr - r   + 0.5f) * scs };
            DrawRectanglePro(bandRec, bandOrigin, p.currentAngle, highlight);

            // Dark bevel at bottom of cell
            Rectangle edgeRec = { p.currentCenterX, p.currentCenterY, scs, edgeH };
            Vector2 edgeOrigin = { (cc - col + 0.5f) * scs,
                                   (cr - r   + 0.5f) * scs - (scs - edgeH) };
            DrawRectanglePro(edgeRec, edgeOrigin, p.currentAngle, edge);
        }
    }
}

void drawParts(const Game& g, const Layout& L) {
    // Two passes so shadows never overlap other parts' bodies.
    for (size_t i = 0; i < g.parts.size(); ++i) {
        const Part& p = g.parts[i];
        const bool held = (static_cast<int>(i) == g.heldPartIdx);
        const bool onBoard = p.location.placed || (held && g.cursorCol != Game::TRAY_COL);
        if (onBoard) drawPartShadow(p, static_cast<float>(L.cellSize), 1.0f);
    }
    for (size_t i = 0; i < g.parts.size(); ++i) {
        const Part& p = g.parts[i];
        const bool held = (static_cast<int>(i) == g.heldPartIdx);
        const bool inTray = !p.location.placed && !held;
        const Color fill = colorBadge(p.colorIndex);
        const float alpha = inTray ? 0.85f : 1.0f;
        drawPartAnimated(p, static_cast<float>(L.cellSize), fill, alpha);
    }
}

} // namespace

Renderer::Renderer() {
    uiFont = LoadFontEx("assets/fonts/Exo2-Regular.ttf", 64, nullptr, 0);
    hasUiFont = IsFontValid(uiFont);
    if (hasUiFont) {
        SetTextureFilter(uiFont.texture, TEXTURE_FILTER_BILINEAR);
    } else {
        uiFont = GetFontDefault();
    }
}

Renderer::~Renderer() {
    if (hasUiFont) {
        UnloadFont(uiFont);
    }
}

void Renderer::draw(Game& g, int screenW, int screenH, float dt) {
    Font font = hasUiFont ? uiFont : GetFontDefault();

    // Vertical gradient bg — darker top fades to deeper bottom.
    // Shared across game / menu / editor (see ui/Theme.h).
    ClearBackground(theme::kBgGradBottom);
    DrawRectangleGradientV(0, 0, screenW, screenH,
                           theme::kBgGradTop, theme::kBgGradBottom);

    const Layout L = computeLayout(g, screenW, screenH);

    animateParts(g, L, dt);

    drawSidebar(g, L, font);
    drawConstraints(g, L, font);
    drawBoardBg(g, L, font);
    drawHints(g, L);
    drawPlacementHighlight(g, L);
    drawCursor(g, L);
    drawTrayBg(g, L, font);
    drawParts(g, L);
    drawStatus(g, screenW, screenH, font);

    // Ease the win banner in; reset when not in a won state so a replay re-plays it.
    if (g.won) {
        winAnim += (1.0f - winAnim) * (1.0f - std::exp(-dt * 10.0f));
        drawWinBanner(screenW, screenH, font, winAnim);
    } else {
        winAnim = 0.0f;
    }
}
