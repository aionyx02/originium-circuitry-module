// Originium Circuit Repair - raylib UI feel demo
//
// Shows the visual polish raylib can give the real project:
//   - smooth cursor-following parts (lerp position)
//   - rotation animation (lerp angle, R key)
//   - hover lift on tray parts
//   - soft drop shadow that grows with lift
//   - rounded cells with top highlight and edge tint
//   - background gradient + row/col hint numbers
//
// Build (macOS, Homebrew raylib):
//   clang++ -std=c++17 -O2 demo/main.cpp -o demo/demo $(pkg-config --libs --cflags raylib)
// Run:
//   ./demo/demo

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

namespace {

constexpr int   WIN_W       = 1100;
constexpr int   WIN_H       = 720;
constexpr int   BOARD_ROWS  = 5;
constexpr int   BOARD_COLS  = 5;
constexpr int   CELL        = 72;
constexpr int   BOARD_X     = 130;
constexpr int   BOARD_Y     = 170;
constexpr int   TRAY_X      = 700;
constexpr int   TRAY_Y      = 210;

const Color BG_TOP    = { 24,  28,  42, 255 };
const Color BG_BOT    = { 12,  14,  24, 255 };
const Color CELL_BG   = { 38,  45,  65, 255 };
const Color CELL_HOV  = { 70,  90, 130, 255 };
const Color TEXT_PRI  = { 235, 240, 250, 255 };
const Color TEXT_DIM  = { 140, 155, 180, 255 };
const Color GREEN_PT  = {  74, 222, 128, 255 };
const Color BLUE_PT   = {  56, 189, 248, 255 };
const Color ORANGE_PT = { 251, 146,  60, 255 };
const Color SHADOW    = {   0,   0,   0,  90 };
const Color TRAY_PANE = { 255, 255, 255,   8 };

unsigned char clampU(int v) {
    return static_cast<unsigned char>(std::max(0, std::min(255, v)));
}

struct Part {
    std::vector<std::pair<int, int>> cells;  // (row, col) offsets, unrotated
    Vector2 pivot{};                          // centroid in cell space (x=col, y=row)
    Color   color{};
    Vector2 homePos{};
    Vector2 renderPos{};
    Vector2 dragOffset{};
    float   targetAngle  = 0.0f;
    float   renderAngle  = 0.0f;
    float   targetLift   = 0.0f;
    float   liftAmount   = 0.0f;
    bool    placed       = false;
    int     placedRow    = 0;
    int     placedCol    = 0;
    int     rotationSteps = 0;
};

Vector2 centroid(const std::vector<std::pair<int, int>>& cells) {
    float sr = 0.0f, sc = 0.0f;
    for (const auto& [r, c] : cells) { sr += r; sc += c; }
    const float n = static_cast<float>(cells.size());
    return { sc / n, sr / n };  // (x = col mean, y = row mean)
}

Part makePart(std::vector<std::pair<int, int>> cells, Color color, Vector2 home) {
    Part p;
    p.cells     = std::move(cells);
    p.pivot     = centroid(p.cells);
    p.color     = color;
    p.homePos   = home;
    p.renderPos = home;
    return p;
}

// Hit test in the part's local frame (un-rotate the mouse point, then check cells).
bool pointInPart(const Part& p, Vector2 point) {
    const float ang = -p.renderAngle * DEG2RAD;
    const float dx  = point.x - p.renderPos.x;
    const float dy  = point.y - p.renderPos.y;
    const float lx  = dx * cosf(ang) - dy * sinf(ang);
    const float ly  = dx * sinf(ang) + dy * cosf(ang);
    for (const auto& [r, c] : p.cells) {
        const float cx = (c - p.pivot.x) * CELL;
        const float cy = (r - p.pivot.y) * CELL;
        if (lx >= cx - CELL * 0.5f && lx < cx + CELL * 0.5f &&
            ly >= cy - CELL * 0.5f && ly < cy + CELL * 0.5f) {
            return true;
        }
    }
    return false;
}

Vector2 boardCellCenter(int r, int c) {
    return {
        static_cast<float>(BOARD_X + c * CELL + CELL / 2),
        static_cast<float>(BOARD_Y + r * CELL + CELL / 2),
    };
}

bool boardCellFromPoint(Vector2 p, int& outR, int& outC) {
    const int c = static_cast<int>((p.x - BOARD_X) / CELL);
    const int r = static_cast<int>((p.y - BOARD_Y) / CELL);
    if (r < 0 || r >= BOARD_ROWS || c < 0 || c >= BOARD_COLS) return false;
    outR = r;
    outC = c;
    return true;
}

void drawPart(const Part& p) {
    // Shadow — offset and scale grow with lift to read as "lifted off the board"
    rlPushMatrix();
    rlTranslatef(p.renderPos.x + 4.0f + 10.0f * p.liftAmount,
                 p.renderPos.y + 6.0f + 14.0f * p.liftAmount,
                 0.0f);
    rlRotatef(p.renderAngle, 0, 0, 1);
    for (const auto& [r, c] : p.cells) {
        const float x = (c - p.pivot.x) * CELL - CELL * 0.5f;
        const float y = (r - p.pivot.y) * CELL - CELL * 0.5f;
        DrawRectangleRounded({ x, y, (float)CELL, (float)CELL }, 0.22f, 8, SHADOW);
    }
    rlPopMatrix();

    // Body
    rlPushMatrix();
    rlTranslatef(p.renderPos.x, p.renderPos.y, 0.0f);
    rlRotatef(p.renderAngle, 0, 0, 1);
    const float s = 1.0f + 0.08f * p.liftAmount;
    rlScalef(s, s, 1.0f);
    for (const auto& [r, c] : p.cells) {
        const float x = (c - p.pivot.x) * CELL - CELL * 0.5f;
        const float y = (r - p.pivot.y) * CELL - CELL * 0.5f;
        const Rectangle inner = { x + 2, y + 2, (float)CELL - 4, (float)CELL - 4 };

        DrawRectangleRounded(inner, 0.22f, 8, p.color);

        // glossy top highlight band
        const Color hl = {
            clampU(p.color.r + 40), clampU(p.color.g + 40), clampU(p.color.b + 40), 180
        };
        DrawRectangleRounded(
            { inner.x + 4, inner.y + 4, inner.width - 8, inner.height * 0.30f },
            0.45f, 6, hl);

        // darker edge tint to give the cell weight
        const Color edge = {
            clampU(p.color.r - 50), clampU(p.color.g - 50), clampU(p.color.b - 50), 255
        };
        DrawRectangleRoundedLines(inner, 0.22f, 8, edge);
    }
    rlPopMatrix();
}

}  // namespace

int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(WIN_W, WIN_H, "Originium Circuit Repair - raylib UI feel demo");
    SetTargetFPS(60);

    std::vector<Part> parts;
    parts.push_back(makePart(
        { {0,0}, {1,0}, {1,1}, {1,2} },           // L-piece
        GREEN_PT,
        { (float)(TRAY_X + 100), (float)(TRAY_Y +  60) }));
    parts.push_back(makePart(
        { {0,0}, {0,1}, {0,2} },                  // I-piece (3 wide)
        BLUE_PT,
        { (float)(TRAY_X + 100), (float)(TRAY_Y + 200) }));
    parts.push_back(makePart(
        { {0,0}, {0,1}, {1,0}, {1,1} },           // O-piece (2x2)
        ORANGE_PT,
        { (float)(TRAY_X + 100), (float)(TRAY_Y + 330) }));

    int dragIdx = -1;

    const int rowHints[BOARD_ROWS] = { 3, 2, 1, 2, 3 };
    const int colHints[BOARD_COLS] = { 2, 1, 3, 2, 3 };

    while (!WindowShouldClose()) {
        const Vector2 mouse = GetMousePosition();

        // ---- begin drag ----
        if (dragIdx == -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = static_cast<int>(parts.size()) - 1; i >= 0; --i) {
                if (pointInPart(parts[i], mouse)) {
                    dragIdx = i;
                    parts[i].dragOffset = {
                        parts[i].renderPos.x - mouse.x,
                        parts[i].renderPos.y - mouse.y,
                    };
                    parts[i].targetLift = 1.0f;
                    parts[i].placed     = false;
                    break;
                }
            }
        }

        // ---- update dragged piece ----
        if (dragIdx != -1) {
            Part& p = parts[dragIdx];
            const Vector2 target = { mouse.x + p.dragOffset.x, mouse.y + p.dragOffset.y };
            p.renderPos.x = Lerp(p.renderPos.x, target.x, 0.40f);
            p.renderPos.y = Lerp(p.renderPos.y, target.y, 0.40f);

            if (IsKeyPressed(KEY_R)) {
                p.targetAngle += 90.0f;
                p.rotationSteps = (p.rotationSteps + 1) % 4;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                p.targetLift = 0.0f;
                dragIdx = -1;
            } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                int br, bc;
                if (boardCellFromPoint(p.renderPos, br, bc)) {
                    p.renderPos = boardCellCenter(br, bc);
                    p.placed    = true;
                    p.placedRow = br;
                    p.placedCol = bc;
                }
                p.targetLift = 0.0f;
                dragIdx = -1;
            }
        }

        // ---- non-drag: ease toward home, hover lift ----
        for (int i = 0; i < (int)parts.size(); ++i) {
            if (i == dragIdx) continue;
            Part& p = parts[i];
            if (!p.placed) {
                p.renderPos.x = Lerp(p.renderPos.x, p.homePos.x, 0.18f);
                p.renderPos.y = Lerp(p.renderPos.y, p.homePos.y, 0.18f);
                p.targetLift  = (dragIdx == -1 && pointInPart(p, mouse)) ? 0.35f : 0.0f;
            } else {
                p.targetLift = 0.0f;
            }
        }

        // ---- smooth angle/lift for every part ----
        for (auto& p : parts) {
            p.renderAngle = Lerp(p.renderAngle, p.targetAngle, 0.18f);
            p.liftAmount  = Lerp(p.liftAmount,  p.targetLift,  0.18f);
        }

        // ============ DRAW ============
        BeginDrawing();
        DrawRectangleGradientV(0, 0, WIN_W, WIN_H, BG_TOP, BG_BOT);

        // Header
        DrawText("Originium Circuit Repair", BOARD_X - 10, 40, 34, TEXT_PRI);
        DrawText("raylib UI feel demo  -  drag to move,  R to rotate,  Esc to release",
                 BOARD_X - 10, 82, 16, TEXT_DIM);

        // Board cells
        for (int r = 0; r < BOARD_ROWS; ++r) {
            for (int c = 0; c < BOARD_COLS; ++c) {
                const Rectangle cellRect = {
                    (float)(BOARD_X + c * CELL + 2),
                    (float)(BOARD_Y + r * CELL + 2),
                    (float)CELL - 4, (float)CELL - 4,
                };
                Color bg = CELL_BG;
                if (dragIdx != -1) {
                    int dr, dc;
                    if (boardCellFromPoint(parts[dragIdx].renderPos, dr, dc) &&
                        dr == r && dc == c) {
                        bg = CELL_HOV;
                    }
                }
                DrawRectangleRounded(cellRect, 0.15f, 6, bg);
            }
        }

        // Row / column hint numbers (green channel only, for visual demo)
        for (int i = 0; i < BOARD_ROWS; ++i) {
            DrawText(TextFormat("%d", rowHints[i]),
                     BOARD_X - 36, BOARD_Y + i * CELL + CELL / 2 - 12, 24, GREEN_PT);
        }
        for (int i = 0; i < BOARD_COLS; ++i) {
            DrawText(TextFormat("%d", colHints[i]),
                     BOARD_X + i * CELL + CELL / 2 - 6, BOARD_Y - 34, 24, GREEN_PT);
        }

        // Tray panel
        DrawRectangleRounded(
            { (float)TRAY_X - 30, (float)TRAY_Y - 30, 260.0f, 460.0f },
            0.05f, 8, TRAY_PANE);
        DrawText("PARTS", TRAY_X + 70, TRAY_Y - 60, 20, TEXT_DIM);

        // Parts (dragged one drawn last so it's on top)
        for (int i = 0; i < (int)parts.size(); ++i) {
            if (i != dragIdx) drawPart(parts[i]);
        }
        if (dragIdx != -1) drawPart(parts[dragIdx]);

        // Footer + FPS
        DrawText("smooth follow  +  rotation tween  +  lift-on-hover  /  built with raylib",
                 BOARD_X - 10, WIN_H - 38, 14, TEXT_DIM);
        DrawText(TextFormat("%d FPS", GetFPS()), WIN_W - 80, 14, 14, TEXT_DIM);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
