#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "raylib.h"

#include "core/Editor.h"
#include "core/Game.h"
#include "core/LevelWriter.h"
#include "core/Parser.h"
#include "ui/Input.h"
#include "ui/Renderer.h"

namespace {

struct LevelEntry {
    std::filesystem::path path;
    std::string name;
};

enum class AppState {
    Menu,
    InGame,
    Editor,
};

std::string promptPath(int argc, char** argv) {
    if (argc > 1 && argv[1] != nullptr) {
        return argv[1];
    }
    std::cout << "Enter level file path: " << std::flush;
    std::string path;
    if (!std::getline(std::cin, path)) return {};
    return path;
}

bool loadGame(const std::string& path, Game& game, std::string& errorOut) {
    std::ifstream in(path);
    if (!in) {
        errorOut = "Cannot open file: " + path;
        return false;
    }
    try {
        GameData data = Parser::parse(in);
        game.init(std::move(data.board), std::move(data.parts));
        return true;
    } catch (const Parser::Error& e) {
        errorOut = std::string("Parse error: ") + e.what();
        return false;
    } catch (const std::exception& e) {
        errorOut = std::string("Unexpected error: ") + e.what();
        return false;
    }
}

constexpr int kMenuPanelW = 560;
constexpr int kMenuPanelH = 456;
constexpr int kMenuListOffsetX = 44;
constexpr int kMenuListOffsetY = 168;
constexpr int kMenuRowH = 42;
constexpr int kMenuVisibleRows = 6;

int menuVisibleStart(int selected, int levelCount) {
    if (levelCount <= kMenuVisibleRows) return 0;
    if (selected < 0) return 0;
    int start = selected - kMenuVisibleRows / 2;
    start = std::max(0, start);
    start = std::min(start, levelCount - kMenuVisibleRows);
    return start;
}

std::vector<LevelEntry> findLevels() {
    namespace fs = std::filesystem;

    std::vector<LevelEntry> levels;
    const fs::path root = "assets/levels";
    if (!fs::exists(root) || !fs::is_directory(root)) return levels;

    for (const auto& entry : fs::directory_iterator(root)) {
        if (!entry.is_regular_file()) continue;
        const fs::path path = entry.path();
        if (path.extension() != ".txt") continue;
        levels.push_back(LevelEntry{path, path.filename().string()});
    }

    std::sort(levels.begin(), levels.end(), [](const LevelEntry& a, const LevelEntry& b) {
        return a.name < b.name;
    });
    return levels;
}

int menuSelectionFromMouse(const std::vector<LevelEntry>& levels,
                           int selected,
                           int screenW,
                           int screenH) {
    if (levels.empty()) return -1;

    const Vector2 m = GetMousePosition();
    const int panelX = (screenW - kMenuPanelW) / 2;
    const int panelY = (screenH - kMenuPanelH) / 2;
    const int listX = panelX + kMenuListOffsetX;
    const int listY = panelY + kMenuListOffsetY;
    const int listW = kMenuPanelW - 2 * kMenuListOffsetX;
    const int maxVisible = std::min(static_cast<int>(levels.size()), kMenuVisibleRows);

    if (m.x < listX || m.x >= listX + listW) return -1;
    if (m.y < listY || m.y >= listY + maxVisible * kMenuRowH) return -1;
    const int localIdx = static_cast<int>(m.y - listY) / kMenuRowH;
    const int idx = menuVisibleStart(selected, static_cast<int>(levels.size())) + localIdx;
    return idx >= 0 && idx < static_cast<int>(levels.size()) ? idx : -1;
}

void drawMenu(const std::vector<LevelEntry>& levels,
              int selected,
              int hover,
              const std::string& message,
              Font font,
              int screenW,
              int screenH) {
    DrawRectangleGradientV(0, 0, screenW, screenH,
                           Color{22, 29, 43, 255},
                           Color{9, 12, 20, 255});

    const int panelX = (screenW - kMenuPanelW) / 2;
    const int panelY = (screenH - kMenuPanelH) / 2;
    Rectangle panel = {
        static_cast<float>(panelX),
        static_cast<float>(panelY),
        static_cast<float>(kMenuPanelW),
        static_cast<float>(kMenuPanelH),
    };
    DrawRectangleRounded(panel, 0.06f, 8, Color{18, 22, 32, 230});
    DrawRectangleRoundedLinesEx(panel, 0.06f, 8, 1.0f, Color{72, 94, 112, 255});

    DrawTextEx(font, "ORIGINIUM", Vector2{static_cast<float>(panelX + 42), static_cast<float>(panelY + 34)},
               32.0f, 1.5f, Color{232, 244, 245, 255});
    DrawTextEx(font, "CIRCUIT REPAIR", Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + 70)},
               18.0f, 1.2f, Color{109, 236, 218, 255});
    DrawRectangle(panelX + 44, panelY + 103, 88, 2, Color{109, 236, 218, 190});
    DrawTextEx(font, "SELECT LEVEL", Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + 132)},
               18.0f, 1.4f, Color{174, 191, 203, 255});

    if (levels.empty()) {
        DrawTextEx(font, "No playable .txt levels found.",
                   Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + 180)},
                   20.0f, 1.0f, Color{220, 120, 120, 255});
    } else {
        const int listX = panelX + kMenuListOffsetX;
        const int listY = panelY + kMenuListOffsetY;
        const int listW = kMenuPanelW - 2 * kMenuListOffsetX;
        const int levelCount = static_cast<int>(levels.size());
        const int start = menuVisibleStart(selected, levelCount);
        const int maxVisible = std::min(levelCount, kMenuVisibleRows);
        for (int localIdx = 0; localIdx < maxVisible; ++localIdx) {
            const int i = start + localIdx;
            Rectangle row = {
                static_cast<float>(listX),
                static_cast<float>(listY + localIdx * kMenuRowH),
                static_cast<float>(listW),
                static_cast<float>(kMenuRowH - 8),
            };
            const bool isSelected = (i == selected);
            const bool isHover = (i == hover);
            DrawRectangleRounded(row, 0.10f, 6,
                                 isSelected ? Color{36, 55, 61, 255}
                                 : isHover ? Color{36, 44, 56, 245}
                                            : Color{31, 37, 48, 230});
            DrawRectangleRoundedLinesEx(row, 0.10f, 6, isSelected ? 2.0f : 1.0f,
                                        isSelected ? Color{109, 236, 218, 255}
                                        : isHover ? Color{104, 130, 150, 245}
                                                   : Color{75, 88, 104, 255});
            DrawTextEx(font, levels[i].name.c_str(),
                       Vector2{static_cast<float>(listX + 18),
                               static_cast<float>(listY + localIdx * kMenuRowH + 7)},
                       20.0f, 1.0f, Color{232, 244, 245, 255});
        }
        if (levelCount > maxVisible) {
            char range[64];
            std::snprintf(range, sizeof(range), "Showing %d-%d of %d",
                          start + 1, start + maxVisible, levelCount);
            DrawTextEx(font, range,
                       Vector2{static_cast<float>(listX),
                               static_cast<float>(listY + maxVisible * kMenuRowH + 6)},
                       14.0f, 1.0f, Color{114, 128, 139, 255});
        }
    }

    DrawTextEx(font, "Enter / Click: start    Up/Down: select    E: create level    N: menu (in game)",
               Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + kMenuPanelH - 58)},
               15.0f, 1.0f, Color{170, 184, 196, 255});
    if (!message.empty()) {
        DrawTextEx(font, message.c_str(),
                   Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + kMenuPanelH - 30)},
                   15.0f, 1.0f, Color{220, 120, 120, 255});
    }
}

} // namespace

namespace {

int countPlaced(const Game& g) {
    int n = 0;
    for (const auto& p : g.parts) if (p.location.placed) ++n;
    return n;
}

int sumRotateCount(const Game& g) {
    int s = 0;
    for (const auto& p : g.parts) s += p.rotateCount;
    return s;
}

// --- Level editor (Phase 5) ---

// Part-designer paint grid dimension (NxN canvas; trimmed to bbox on add).
constexpr int kPaintN = 5;

Color editorColorBadge(unsigned i) {
    static const Color cs[] = {
        { 76, 175,  80, 255}, { 33, 150, 243, 255},
        {244,  67,  54, 255}, {255, 193,   7, 255},
    };
    return cs[i % 4];
}

void editorCenteredText(Font font, const char* s, float cx, float cy, float fs, Color col) {
    const Vector2 sz = MeasureTextEx(font, s, fs, 1.0f);
    DrawTextEx(font, s, Vector2{cx - sz.x / 2.0f, cy - sz.y / 2.0f}, fs, 1.0f, col);
}

// Write the level to assets/levels/custom-N.txt (next free N). Returns the
// path written, or "" on failure.
std::string exportEditorLevel(const Editor& ed) {
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::create_directories("assets/levels", ec);
    fs::path path;
    for (int n = 1; n < 10000; ++n) {
        path = fs::path("assets/levels") / ("custom-" + std::to_string(n) + ".txt");
        if (!fs::exists(path)) break;
    }
    std::ofstream out(path);
    if (!out) return {};
    out << LevelWriter::write(ed.board, ed.parts);
    if (!out) return {};
    return path.string();
}

enum class EditorResult { Stay, Play, Menu };

// Immediate-mode editor: draws and handles mouse input in one pass. Click a
// board cell to apply the active tool (erase / block / fix); click the number
// boxes around the board to adjust that color's row/column requirement
// (left = +1, right = -1); paint a shape in the designer and ADD it as a part.
EditorResult runEditor(Editor& ed, std::vector<std::vector<bool>>& paint, int& tool,
                       int& selPiece, std::string& msg, Font font, int sw, int sh) {
    const Vector2 mp = GetMousePosition();
    const bool lc = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    const bool rc = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
    const Color cyan = Color{109, 236, 218, 255};
    const Color text = Color{232, 244, 245, 255};
    const Color cur  = editorColorBadge(ed.currentColor);

    auto hit = [&](Rectangle r) { return CheckCollisionPointRec(mp, r); };
    auto button = [&](Rectangle r, const char* label, bool active, float fs = 16.0f) -> bool {
        const bool over = hit(r);
        DrawRectangleRounded(r, 0.22f, 5,
            active ? Color{36, 55, 61, 255} : over ? Color{40, 48, 60, 255} : Color{26, 31, 41, 255});
        DrawRectangleRoundedLinesEx(r, 0.22f, 5, active ? 2.0f : 1.0f,
            active ? cyan : over ? Color{120, 150, 170, 255} : Color{72, 94, 112, 255});
        editorCenteredText(font, label, r.x + r.width / 2, r.y + r.height / 2, fs, text);
        return over && lc;
    };

    EditorResult result = EditorResult::Stay;
    if (IsKeyPressed(KEY_ESCAPE)) result = EditorResult::Menu;

    DrawRectangleGradientV(0, 0, sw, sh, Color{22, 29, 43, 255}, Color{9, 12, 20, 255});

    // ---- Sidebar ----
    Rectangle panel = {24.0f, 20.0f, 320.0f, (float)sh - 40};
    DrawRectangleRounded(panel, 0.04f, 8, Color{18, 22, 32, 235});
    DrawRectangleRoundedLinesEx(panel, 0.04f, 8, 1.0f, Color{72, 94, 112, 255});
    DrawTextEx(font, "LEVEL EDITOR", Vector2{44, 34}, 24.0f, 1.2f, text);
    DrawTextEx(font, "CIRCUIT DESIGNER", Vector2{45, 62}, 13.0f, 1.4f, cyan);

    float y = 92;
    auto stepper = [&](const char* label, int value, int lo, int hi, std::function<void(int)> apply) {
        DrawTextEx(font, label, Vector2{44, y + 5}, 15.0f, 1.0f, Color{174, 191, 203, 255});
        if (button({168, y, 26, 26}, "-", false)) apply(std::max(lo, value - 1));
        char b[8]; std::snprintf(b, sizeof(b), "%d", value);
        editorCenteredText(font, b, 224, y + 13, 18.0f, text);
        if (button({250, y, 26, 26}, "+", false)) apply(std::min(hi, value + 1));
        y += 34;
    };
    stepper("ROWS",   (int)ed.board.rows, 1, Editor::kMaxDim, [&](int v){ ed.setSize(v, ed.board.cols); });
    stepper("COLS",   (int)ed.board.cols, 1, Editor::kMaxDim, [&](int v){ ed.setSize(ed.board.rows, v); });
    stepper("COLORS", (int)ed.board.colors, 1, Editor::kMaxColors, [&](int v){ ed.setColorCount(v); });

    // Color picker.
    y += 4;
    DrawTextEx(font, "COLOR", Vector2{44, y}, 14.0f, 1.2f, Color{150, 166, 180, 255});
    for (int i = 0; i < (int)ed.board.colors; ++i) {
        Rectangle sw_ = {110.0f + i * 40, y - 4, 32, 22};
        DrawRectangleRounded(sw_, 0.3f, 5, editorColorBadge(i));
        if ((int)ed.currentColor == i)
            DrawRectangleRoundedLinesEx(sw_, 0.3f, 5, 2.5f, cyan);
        if (hit(sw_) && lc) ed.currentColor = (unsigned)i;
    }
    y += 34;

    // Board tool: PLACE pieces, or paint BLOCK / FIX / ERASE cells.
    DrawTextEx(font, "BOARD TOOL", Vector2{44, y}, 14.0f, 1.2f, Color{150, 166, 180, 255});
    y += 20;
    if (button({44, y, 70, 28},  "PLACE", tool == 0)) tool = 0;
    if (button({118, y, 70, 28}, "BLOCK", tool == 1)) tool = 1;
    if (button({192, y, 70, 28}, "FIX",   tool == 2)) tool = 2;
    if (button({266, y, 70, 28}, "ERASE", tool == 3)) tool = 3;
    y += 32;
    const char* toolHelp = tool == 0 ? "PLACE: pick a piece below, click board to lay it"
                         : tool == 1 ? "BLOCK: click a cell = no-placement (X)"
                         : tool == 2 ? "FIX: click a cell = fixed piece (current color)"
                                     : "ERASE: click a cell to clear it";
    DrawTextEx(font, toolHelp, Vector2{44, y}, 13.0f, 1.0f, Color{130, 146, 160, 255});
    y += 26;

    // Piece palette — every piece you've made; click one to select for PLACE.
    if (selPiece >= (int)ed.parts.size()) selPiece = (int)ed.parts.size() - 1;
    DrawTextEx(font, "PIECES", Vector2{44, y}, 14.0f, 1.2f, Color{150, 166, 180, 255});
    if (button({200, y - 4, 64, 22}, "ROTATE", false)) ed.rotatePart(selPiece);
    if (button({270, y - 4, 66, 22}, "DELETE", false)) {
        if (selPiece >= 0 && selPiece == (int)ed.parts.size() - 1) ed.removeLastPart();
        selPiece = (int)ed.parts.size() - 1;
    }
    y += 22;
    const int tile = 42;
    const int perRow = 6;
    for (int i = 0; i < (int)ed.parts.size(); ++i) {
        const int tx = 44 + (i % perRow) * (tile + 4);
        const int ty = (int)y + (i / perRow) * (tile + 4);
        Rectangle box = {(float)tx, (float)ty, (float)tile, (float)tile};
        const bool sel = (i == selPiece);
        DrawRectangleRounded(box, 0.14f, 4, Color{26, 31, 41, 255});
        DrawRectangleRoundedLinesEx(box, 0.14f, 4, sel ? 2.5f : 1.0f,
                                    sel ? cyan : Color{72, 94, 112, 255});
        // mini shape preview (current rotation), dimmed if already placed.
        const auto cells = ed.parts[i].rotatedCells();
        int mx = 0, my = 0;
        for (auto [dr, dc] : cells) { mx = std::max(mx, dc); my = std::max(my, dr); }
        const int span = std::max(1, std::max(mx, my) + 1);
        const float cs = (tile - 10.0f) / span;
        Color pcCol = editorColorBadge(ed.parts[i].colorIndex);
        if (ed.parts[i].location.placed) pcCol = Color{(unsigned char)(pcCol.r/2),(unsigned char)(pcCol.g/2),(unsigned char)(pcCol.b/2),255};
        for (auto [dr, dc] : cells)
            DrawRectangleRounded(Rectangle{tx + 5 + dc * cs, (float)ty + 5 + dr * cs, cs - 1, cs - 1}, 0.2f, 3, pcCol);
        if (hit(box) && lc) selPiece = i;
    }
    const int rows_ = (ed.parts.empty() ? 1 : (int)((ed.parts.size() + perRow - 1) / perRow));
    y += rows_ * (tile + 4) + 6;
    if (ed.parts.empty())
        DrawTextEx(font, "(make a piece below)", Vector2{44, y - tile + 6}, 14.0f, 1.0f,
                   Color{120, 134, 148, 255});

    // Make a piece — draw a shape, ADD it to the palette.
    DrawTextEx(font, "MAKE A PIECE", Vector2{44, y}, 14.0f, 1.2f, Color{174, 191, 203, 255});
    DrawRectangleRounded(Rectangle{186, y, 30, 15}, 0.6f, 6, cur);
    y += 20;
    const int pcs = 20, pgx = 56, pgy = (int)y;
    for (int r = 0; r < kPaintN; ++r) {
        for (int c = 0; c < kPaintN; ++c) {
            Rectangle pc = {(float)(pgx + c * pcs), (float)(pgy + r * pcs), (float)(pcs - 3), (float)(pcs - 3)};
            const bool on = paint[r][c];
            DrawRectangleRounded(pc, 0.18f, 4, on ? cur : Color{30, 36, 46, 255});
            DrawRectangleRoundedLinesEx(pc, 0.18f, 4, 1.0f, on ? cyan : Color{70, 82, 98, 255});
            if (hit(pc) && lc) paint[r][c] = !paint[r][c];
        }
    }
    if (button({(float)(pgx + kPaintN * pcs + 12), (float)pgy + 8, 92, 30}, "ADD", false)) {
        const std::size_t before = ed.parts.size();
        ed.addPart(paint, ed.currentColor);
        if (ed.parts.size() > before) {
            for (auto& row : paint) std::fill(row.begin(), row.end(), false);
            selPiece = (int)ed.parts.size() - 1;
            msg = "Piece added — pick PLACE and click the board.";
        } else {
            msg = "Draw a shape in the grid first.";
        }
    }
    y = pgy + kPaintN * pcs + 8;

    // Action buttons (bottom of sidebar).
    const float ay = panel.y + panel.height - 44;
    if (button({44, ay, 84, 30}, "EXPORT", false)) {
        const std::string path = exportEditorLevel(ed);
        msg = path.empty() ? "Export failed." : ("Exported: " + path);
    }
    if (button({134, ay, 84, 30}, "PLAY", false)) result = EditorResult::Play;
    if (button({224, ay, 84, 30}, "MENU", false)) result = EditorResult::Menu;

    // ---- Board + clickable number boxes ----
    // Read dims AFTER the steppers/color count above (they may have resized the
    // board this frame) so the loops below never index a stale, larger grid.
    const int M = static_cast<int>(ed.board.rows);
    const int N = static_cast<int>(ed.board.cols);
    const int availW = sw - 360 - 40;
    const int availH = sh - 170;
    int cell = std::min(availW / std::max(1, N), availH / std::max(1, M));
    cell = std::max(26, std::min(cell, 60));
    const int boardX = 372 + (availW - N * cell) / 2;
    const int boardY = 120 + (availH - M * cell) / 2;
    const int nb = std::min(cell - 2, 30);  // number-box size

    // Board header + how-to.
    DrawTextEx(font, "BOARD", Vector2{(float)(boardX - nb - 6), 46.0f}, 22.0f, 1.2f, text);
    DrawTextEx(font, "Lay your pieces out to form a solution — the row/column",
               Vector2{(float)(boardX - nb - 6), 74.0f}, 14.0f, 1.0f, Color{150, 166, 180, 255});
    DrawTextEx(font, "numbers fill in automatically. Right-click a cell to clear it.",
               Vector2{(float)(boardX - nb - 6), 94.0f}, 14.0f, 1.0f, Color{150, 166, 180, 255});

    // Hovered board cell.
    int hovR = -1, hovC = -1;
    if (mp.x >= boardX && mp.y >= boardY) {
        const int c = (static_cast<int>(mp.x) - boardX) / cell;
        const int r = (static_cast<int>(mp.y) - boardY) / cell;
        if (r >= 0 && r < M && c >= 0 && c < N) { hovR = r; hovC = c; }
    }

    // Auto-derived row/column numbers (read-only).
    auto numberBox = [&](int x, int yy, int idx, bool isRow) {
        Rectangle r = {(float)x, (float)yy, (float)nb, (float)nb};
        DrawRectangleRounded(r, 0.25f, 5, Color{26, 31, 41, 255});
        DrawRectangleRoundedLinesEx(r, 0.25f, 5, 1.0f, cur);
        char b[8]; std::snprintf(b, sizeof(b), "%u", ed.constraintAt(ed.currentColor, idx, isRow));
        editorCenteredText(font, b, x + nb / 2.0f, yy + nb / 2.0f, 17.0f, text);
    };
    for (int r = 0; r < M; ++r)
        numberBox(boardX - nb - 6, boardY + r * cell + (cell - nb) / 2, r, true);
    for (int c = 0; c < N; ++c)
        numberBox(boardX + c * cell + (cell - nb) / 2, boardY - nb - 6, c, false);

    for (int r = 0; r < M; ++r) {
        for (int c = 0; c < N; ++c) {
            Rectangle cellRect = {(float)(boardX + c * cell), (float)(boardY + r * cell),
                                  (float)(cell - 4), (float)(cell - 4)};
            const int v = ed.board._boardInfo[r][c];
            if (v == Board::CANNOT_PLACE) {
                DrawRectangleRounded(cellRect, 0.15f, 6, Color{28, 30, 38, 255});
                DrawRectangleRoundedLinesEx(cellRect, 0.15f, 6, 1.0f, Color{200, 80, 80, 255});
                editorCenteredText(font, "X", cellRect.x + cellRect.width / 2,
                                   cellRect.y + cellRect.height / 2, 22.0f, Color{200, 80, 80, 255});
            } else if (Board::isCannotMove(v)) {
                const Color cc = editorColorBadge((unsigned)Board::cannotMoveColor(v));
                DrawRectangleRounded(cellRect, 0.15f, 6,
                    Color{(unsigned char)(cc.r / 3), (unsigned char)(cc.g / 3), (unsigned char)(cc.b / 3), 255});
                DrawRectangleRoundedLinesEx(cellRect, 0.15f, 6, 1.5f, cc);
                editorCenteredText(font, "=", cellRect.x + cellRect.width / 2,
                                   cellRect.y + cellRect.height / 2, 22.0f, cc);
            } else if (Board::isOccupied(v)) {
                const int pi = Board::occupiedPartIndex(v);
                const Color cc = (pi >= 0 && pi < (int)ed.parts.size())
                               ? editorColorBadge(ed.parts[pi].colorIndex) : Color{120,120,120,255};
                DrawRectangleRounded(cellRect, 0.15f, 6, cc);
                DrawRectangleRoundedLinesEx(cellRect, 0.15f, 6, 1.0f, Color{240,248,250,160});
            } else {
                DrawRectangleRounded(cellRect, 0.15f, 6, Color{45, 50, 62, 255});
                DrawRectangleRoundedLinesEx(cellRect, 0.15f, 6, 1.0f, Color{80, 90, 110, 255});
            }
        }
    }

    // Ghost preview of the selected piece under the cursor (PLACE tool).
    if (tool == 0 && hovR >= 0 && selPiece >= 0 && selPiece < (int)ed.parts.size()
        && !ed.parts[selPiece].location.placed) {
        const bool ok = ed.board.canPlace(ed.parts[selPiece], hovR, hovC);
        const Color g = ok ? Color{80, 220, 100, 110} : Color{220, 80, 80, 110};
        for (auto [dr, dc] : ed.parts[selPiece].rotatedCells()) {
            const int rr = hovR + dr, ccol = hovC + dc;
            if (rr < 0 || ccol < 0 || rr >= M || ccol >= N) continue;
            DrawRectangleRounded(Rectangle{(float)(boardX + ccol * cell), (float)(boardY + rr * cell),
                                           (float)(cell - 4), (float)(cell - 4)}, 0.15f, 6, g);
        }
    } else if (hovR >= 0) {
        DrawRectangleRoundedLinesEx(Rectangle{(float)(boardX + hovC * cell), (float)(boardY + hovR * cell),
                                              (float)(cell - 4), (float)(cell - 4)}, 0.15f, 6, 2.0f, cyan);
    }

    // Clicks on the board.
    if (hovR >= 0) {
        if (lc) {
            if (tool == 0) {
                if (selPiece < 0 || selPiece >= (int)ed.parts.size()) msg = "Make/select a piece first.";
                else if (!ed.placePart(selPiece, hovR, hovC)) msg = "Can't place there.";
            } else if (tool == 1) ed.setBlocked(hovR, hovC);
            else if (tool == 2) ed.setFixed(hovR, hovC, ed.currentColor);
            else ed.clearCell(hovR, hovC);
        }
        if (rc) ed.clearCell(hovR, hovC);
    }

    if (!msg.empty())
        DrawTextEx(font, msg.c_str(), Vector2{(float)boardX, (float)(boardY + M * cell + 16)},
                   16.0f, 1.0f, Color{255, 203, 126, 255});

    return result;
}

} // namespace

int main(int argc, char** argv) {
    Game game;
    std::string err;
    AppState appState = AppState::Menu;
    if (argc > 1 && argv[1] != nullptr) {
        const std::string path = promptPath(argc, argv);
        if (path.empty()) {
            std::cerr << "No level file given." << std::endl;
            return 1;
        }
        if (!loadGame(path, game, err)) {
            std::cerr << err << std::endl;
            return 1;
        }
        appState = AppState::InGame;
    }

    constexpr int kScreenW = 960;
    constexpr int kScreenH = 720;
    constexpr float kHintDelaySec = 30.0f;  // Phase 4: hint appears only after ~30s stuck

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(kScreenW, kScreenH, "Originium Circuit Repair");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    InitAudioDevice();
    Sound sndPickup = LoadSound("assets/sfx/pickup.mp3");
    Sound sndPlace  = LoadSound("assets/sfx/place.mp3");
    Sound sndSpin   = LoadSound("assets/sfx/spin.mp3");
    Sound sndWin    = LoadSound("assets/sfx/victory.mp3");

    int  prevHeld       = -1;
    bool prevWon        = false;
    int  prevPlacedCnt  = 0;
    int  prevRotateSum  = 0;
    float idleTimer     = 0.0f;
    auto resetSoundBaseline = [&]() {
        prevHeld      = game.heldPartIdx;
        prevWon       = game.won;
        prevPlacedCnt = countPlaced(game);
        prevRotateSum = sumRotateCount(game);
        idleTimer     = 0.0f;
    };
    resetSoundBaseline();

    std::vector<LevelEntry> levels = findLevels();
    int selectedLevel = levels.empty() ? -1 : 0;
    std::string menuMessage;

    Editor editor;
    int editorTool = 0;           // 0=place, 1=block, 2=fix, 3=erase
    int editorSelPiece = -1;      // selected piece in the palette
    std::string editorMessage;
    std::vector<std::vector<bool>> paintGrid(kPaintN, std::vector<bool>(kPaintN, false));
    auto clearPaint = [&]() {
        for (auto& row : paintGrid) std::fill(row.begin(), row.end(), false);
    };

    {
        Font menuFont = LoadFontEx("assets/fonts/Exo2-Regular.ttf", 64, nullptr, 0);
        const bool hasMenuFont = IsFontValid(menuFont);
        if (hasMenuFont) {
            SetTextureFilter(menuFont.texture, TEXTURE_FILTER_BILINEAR);
        } else {
            menuFont = GetFontDefault();
        }
        Renderer renderer;
        while (!WindowShouldClose()) {
            const int sw = GetScreenWidth();
            const int sh = GetScreenHeight();

            BeginDrawing();
            if (appState == AppState::Menu) {
                const int hover = menuSelectionFromMouse(levels, selectedLevel, sw, sh);
                const Vector2 mouseDelta = GetMouseDelta();
                const bool mouseMoved = (mouseDelta.x != 0.0f) || (mouseDelta.y != 0.0f);
                if (hover >= 0 && mouseMoved) selectedLevel = hover;

                if (!levels.empty()) {
                    if (IsKeyPressed(KEY_DOWN)) {
                        selectedLevel = (selectedLevel + 1) % static_cast<int>(levels.size());
                    }
                    if (IsKeyPressed(KEY_UP)) {
                        selectedLevel = (selectedLevel + static_cast<int>(levels.size()) - 1)
                                      % static_cast<int>(levels.size());
                    }
                }

                const bool chooseByKey = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
                const bool chooseByMouse = hover >= 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
                if (chooseByMouse) selectedLevel = hover;
                if (!levels.empty() && selectedLevel >= 0 && (chooseByKey || chooseByMouse)) {
                    if (loadGame(levels[selectedLevel].path.string(), game, menuMessage)) {
                        appState = AppState::InGame;
                        resetSoundBaseline();
                        menuMessage.clear();
                    }
                }
                if (IsKeyPressed(KEY_E)) {
                    editor = Editor();
                    editorTool = 0;
                    editorSelPiece = -1;
                    editorMessage.clear();
                    clearPaint();
                    appState = AppState::Editor;
                }
                drawMenu(levels, selectedLevel, hover, menuMessage, menuFont, sw, sh);
            } else if (appState == AppState::Editor) {
                const EditorResult res =
                    runEditor(editor, paintGrid, editorTool, editorSelPiece, editorMessage, menuFont, sw, sh);
                if (res == EditorResult::Play) {
                    game.init(editor.board, editor.parts);
                    appState = AppState::InGame;
                    resetSoundBaseline();
                    editorMessage.clear();
                } else if (res == EditorResult::Menu) {
                    appState = AppState::Menu;
                    levels = findLevels();   // surface any just-exported level
                    if (selectedLevel < 0 && !levels.empty()) selectedLevel = 0;
                    editorMessage.clear();
                }
            } else {
                if (IsKeyPressed(KEY_N)) {
                    appState = AppState::Menu;
                    levels = findLevels();
                    if (selectedLevel < 0 && !levels.empty()) selectedLevel = 0;
                    if (selectedLevel >= static_cast<int>(levels.size())) {
                        selectedLevel = levels.empty() ? -1 : 0;
                    }
                    menuMessage.clear();
                    drawMenu(levels, selectedLevel, -1, menuMessage, menuFont, sw, sh);
                    EndDrawing();
                    continue;
                }

                const Layout L = computeLayout(game, sw, sh);
                const Action a = Input::poll();
                if (a != Action::None) game.update(a);
                Input::pollMouse(L, game);

                const int  heldNow      = game.heldPartIdx;
                const bool wonNow       = game.won;
                const int  placedCntNow = countPlaced(game);
                const int  rotateSumNow = sumRotateCount(game);

                // One sound per frame, priority: win > place > pickup > spin.
                if (wonNow && !prevWon) {
                    if (IsSoundValid(sndWin)) PlaySound(sndWin);
                } else if (placedCntNow > prevPlacedCnt) {
                    if (IsSoundValid(sndPlace)) PlaySound(sndPlace);
                } else if (prevHeld < 0 && heldNow >= 0) {
                    if (IsSoundValid(sndPickup)) PlaySound(sndPickup);
                } else if (rotateSumNow > prevRotateSum) {
                    if (IsSoundValid(sndSpin)) PlaySound(sndSpin);
                }

                // Phase 4: reveal the solution hint only after ~30s with no
                // placement change. Any progress (place/remove) or a win resets
                // the timer and hides the hint again.
                if (placedCntNow != prevPlacedCnt || wonNow) idleTimer = 0.0f;
                else idleTimer += GetFrameTime();
                game.setHintsVisible(!wonNow && idleTimer >= kHintDelaySec);

                prevHeld      = heldNow;
                prevWon       = wonNow;
                prevPlacedCnt = placedCntNow;
                prevRotateSum = rotateSumNow;

                renderer.draw(game, sw, sh, GetFrameTime());
            }
            EndDrawing();
        }
        if (hasMenuFont) {
            UnloadFont(menuFont);
        }
    }

    UnloadSound(sndPickup);
    UnloadSound(sndPlace);
    UnloadSound(sndSpin);
    UnloadSound(sndWin);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
