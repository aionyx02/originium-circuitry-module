#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <filesystem>
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

// Part-designer paint grid (sidebar, lower-left). Shared by draw + hit-test.
constexpr int kPaintN    = 5;
constexpr int kPaintCell = 30;
constexpr int kPaintX    = 60;
constexpr int kPaintY    = 396;

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

void drawEditor(const Editor& ed, int selIdx, const std::vector<std::vector<bool>>& paint,
                const std::string& msg, Font font, int sw, int sh) {
    DrawRectangleGradientV(0, 0, sw, sh, Color{22, 29, 43, 255}, Color{9, 12, 20, 255});

    const int M = static_cast<int>(ed.board.rows);
    const int N = static_cast<int>(ed.board.cols);
    const int cell = 56;
    const int boardX = (sw - N * cell) / 2 + 90;
    const int boardY = (sh - M * cell) / 2 + 10;
    const Color cur = editorColorBadge(ed.currentColor);
    const Color cyan = Color{109, 236, 218, 255};

    // Board cells.
    for (int r = 0; r < M; ++r) {
        for (int c = 0; c < N; ++c) {
            Rectangle rc = {(float)(boardX + c * cell), (float)(boardY + r * cell),
                            (float)(cell - 4), (float)(cell - 4)};
            DrawRectangleRounded(rc, 0.15f, 6, Color{45, 50, 62, 255});
            DrawRectangleRoundedLinesEx(rc, 0.15f, 6, 1.0f, Color{80, 90, 110, 255});
        }
    }

    // Constraint slots for the current color: row reqs (left), col reqs (top).
    const int box = 38;
    auto drawSlot = [&](int x, int y, unsigned val, bool sel) {
        Rectangle r = {(float)x, (float)y, (float)box, (float)box};
        DrawRectangleRounded(r, 0.2f, 5, sel ? Color{36, 55, 61, 255} : Color{26, 31, 41, 255});
        DrawRectangleRoundedLinesEx(r, 0.2f, 5, sel ? 2.0f : 1.0f, sel ? cyan : cur);
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%u", val);
        editorCenteredText(font, buf, x + box / 2.0f, y + box / 2.0f, 20.0f,
                           Color{232, 244, 245, 255});
    };
    for (int r = 0; r < M; ++r) {
        drawSlot(boardX - box - 8, boardY + r * cell + (cell - box) / 2,
                 ed.constraintAt(ed.currentColor, r, true), selIdx == r);
    }
    for (int c = 0; c < N; ++c) {
        drawSlot(boardX + c * cell + (cell - box) / 2, boardY - box - 8,
                 ed.constraintAt(ed.currentColor, c, false), selIdx == M + c);
    }

    // Sidebar — params + key legend.
    Rectangle panel = {24.0f, 20.0f, 300.0f, 300.0f};
    DrawRectangleRounded(panel, 0.06f, 8, Color{18, 22, 32, 230});
    DrawRectangleRoundedLinesEx(panel, 0.06f, 8, 1.0f, Color{72, 94, 112, 255});
    DrawTextEx(font, "LEVEL EDITOR", Vector2{44, 36}, 26.0f, 1.2f, Color{232, 244, 245, 255});

    char line[96];
    std::snprintf(line, sizeof(line), "Size: %d x %d     Colors: %u", M, N, ed.board.colors);
    DrawTextEx(font, line, Vector2{44, 86}, 18.0f, 1.0f, Color{174, 191, 203, 255});

    DrawTextEx(font, "Editing color:", Vector2{44, 116}, 18.0f, 1.0f, Color{174, 191, 203, 255});
    DrawRectangleRounded(Rectangle{196, 118, 40, 18}, 0.6f, 6, cur);

    const char* help[] = {
        "Z/X rows   C/V cols",
        "K/L colors   Tab color",
        "Up/Down pick slot",
        "Left/Right value -/+",
        "A add part   D del last",
        "E export   P play   Esc menu",
    };
    float y = 156;
    for (const char* h : help) {
        DrawTextEx(font, h, Vector2{44, y}, 16.0f, 1.0f, Color{150, 166, 180, 255});
        y += 26;
    }

    // Part-designer panel — paint a shape, A adds it as a part of the current color.
    Rectangle dpanel = {24.0f, 332.0f, 300.0f, 368.0f};
    DrawRectangleRounded(dpanel, 0.06f, 8, Color{18, 22, 32, 230});
    DrawRectangleRoundedLinesEx(dpanel, 0.06f, 8, 1.0f, Color{72, 94, 112, 255});
    DrawTextEx(font, "PART DESIGNER", Vector2{44, 346}, 18.0f, 1.2f, Color{174, 191, 203, 255});
    DrawRectangleRounded(Rectangle{210, 348, 40, 16}, 0.6f, 6, cur);

    for (int r = 0; r < kPaintN; ++r) {
        for (int c = 0; c < kPaintN; ++c) {
            Rectangle pc = {(float)(kPaintX + c * kPaintCell), (float)(kPaintY + r * kPaintCell),
                            (float)(kPaintCell - 3), (float)(kPaintCell - 3)};
            const bool on = r < (int)paint.size() && c < (int)paint[r].size() && paint[r][c];
            DrawRectangleRounded(pc, 0.18f, 4, on ? cur : Color{30, 36, 46, 255});
            DrawRectangleRoundedLinesEx(pc, 0.18f, 4, 1.0f, on ? cyan : Color{70, 82, 98, 255});
        }
    }

    char pcount[64];
    std::snprintf(pcount, sizeof(pcount), "PARTS: %d", (int)ed.parts.size());
    DrawTextEx(font, pcount, Vector2{44, (float)(kPaintY + kPaintN * kPaintCell + 8)}, 16.0f, 1.0f,
               Color{232, 244, 245, 255});
    for (int i = 0; i < (int)ed.parts.size() && i < 14; ++i) {
        DrawRectangleRounded(Rectangle{140.0f + i * 13, (float)(kPaintY + kPaintN * kPaintCell + 8),
                                       11, 14}, 0.5f, 4, editorColorBadge(ed.parts[i].colorIndex));
    }

    if (!msg.empty()) {
        DrawTextEx(font, msg.c_str(),
                   Vector2{(float)boardX, (float)(boardY + M * cell + 18)}, 16.0f, 1.0f,
                   Color{255, 203, 126, 255});
    }
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
    int editorSel = 0;            // 0..M-1 = row slot, M..M+N-1 = col slot
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
                    editorSel = 0;
                    editorMessage.clear();
                    clearPaint();
                    appState = AppState::Editor;
                }
                drawMenu(levels, selectedLevel, hover, menuMessage, menuFont, sw, sh);
            } else if (appState == AppState::Editor) {
                const int M = static_cast<int>(editor.board.rows);
                const int N = static_cast<int>(editor.board.cols);
                const int slotCount = M + N;

                if (IsKeyPressed(KEY_Z)) editor.setSize(M - 1, N);
                if (IsKeyPressed(KEY_X)) editor.setSize(M + 1, N);
                if (IsKeyPressed(KEY_C)) editor.setSize(M, N - 1);
                if (IsKeyPressed(KEY_V)) editor.setSize(M, N + 1);
                if (IsKeyPressed(KEY_K)) editor.setColorCount(static_cast<int>(editor.board.colors) - 1);
                if (IsKeyPressed(KEY_L)) editor.setColorCount(static_cast<int>(editor.board.colors) + 1);
                if (IsKeyPressed(KEY_TAB) && editor.board.colors > 1) {
                    editor.currentColor = (editor.currentColor + 1) % editor.board.colors;
                }

                // Re-clamp selection (size may have changed this frame).
                if (editorSel >= slotCount) editorSel = std::max(0, slotCount - 1);
                if (IsKeyPressed(KEY_DOWN)) editorSel = (editorSel + 1) % std::max(1, slotCount);
                if (IsKeyPressed(KEY_UP))   editorSel = (editorSel + slotCount - 1) % std::max(1, slotCount);

                const bool isRow = editorSel < M;
                const int  idx   = isRow ? editorSel : editorSel - M;
                if (IsKeyPressed(KEY_RIGHT)) editor.adjustConstraint(editor.currentColor, idx, isRow, +1);
                if (IsKeyPressed(KEY_LEFT))  editor.adjustConstraint(editor.currentColor, idx, isRow, -1);

                // Part designer: click paints a cell; A adds the shape as a part, D deletes last.
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    const Vector2 mp = GetMousePosition();
                    const int pcc = (static_cast<int>(mp.x) - kPaintX) / kPaintCell;
                    const int prr = (static_cast<int>(mp.y) - kPaintY) / kPaintCell;
                    if (mp.x >= kPaintX && mp.y >= kPaintY &&
                        prr >= 0 && prr < kPaintN && pcc >= 0 && pcc < kPaintN) {
                        paintGrid[prr][pcc] = !paintGrid[prr][pcc];
                    }
                }
                if (IsKeyPressed(KEY_A)) {
                    const std::size_t before = editor.parts.size();
                    editor.addPart(paintGrid, editor.currentColor);
                    if (editor.parts.size() > before) {
                        clearPaint();
                        editorMessage = "Part added.";
                    } else {
                        editorMessage = "Paint a shape in the designer first.";
                    }
                }
                if (IsKeyPressed(KEY_D)) {
                    editor.removeLastPart();
                    editorMessage = "Removed last part.";
                }

                if (IsKeyPressed(KEY_E)) {
                    const std::string path = exportEditorLevel(editor);
                    editorMessage = path.empty() ? "Export failed." : ("Exported: " + path);
                }
                if (IsKeyPressed(KEY_P)) {
                    game.init(editor.board, editor.parts);
                    appState = AppState::InGame;
                    resetSoundBaseline();
                    editorMessage.clear();
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    appState = AppState::Menu;
                    levels = findLevels();   // surface any just-exported level
                    if (selectedLevel < 0 && !levels.empty()) selectedLevel = 0;
                    editorMessage.clear();
                    drawMenu(levels, selectedLevel, -1, menuMessage, menuFont, sw, sh);
                    EndDrawing();
                    continue;
                }

                drawEditor(editor, editorSel, paintGrid, editorMessage, menuFont, sw, sh);
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
