#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

#include "raylib.h"

#include "core/Game.h"
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

std::vector<LevelEntry> findLevels() {
    namespace fs = std::filesystem;

    std::vector<LevelEntry> levels;
    const fs::path preferred = "assets/levels";
    const fs::path fallback = "docs/io";
    const fs::path root = fs::exists(preferred) ? preferred : fallback;
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

int menuSelectionFromMouse(const std::vector<LevelEntry>& levels, int screenW, int screenH) {
    if (levels.empty()) return -1;

    const Vector2 m = GetMousePosition();
    const int panelW = 560;
    const int panelH = 456;
    const int panelX = (screenW - panelW) / 2;
    const int panelY = (screenH - panelH) / 2;
    const int listX = panelX + 44;
    const int listY = panelY + 168;
    const int listW = panelW - 88;
    const int rowH = 42;
    const int maxVisible = std::min(static_cast<int>(levels.size()), 6);

    if (m.x < listX || m.x >= listX + listW) return -1;
    if (m.y < listY || m.y >= listY + maxVisible * rowH) return -1;
    const int idx = static_cast<int>(m.y - listY) / rowH;
    return idx >= 0 && idx < maxVisible ? idx : -1;
}

void drawMenu(const std::vector<LevelEntry>& levels,
              int selected,
              const std::string& message,
              Font font,
              int screenW,
              int screenH) {
    DrawRectangleGradientV(0, 0, screenW, screenH,
                           Color{22, 29, 43, 255},
                           Color{9, 12, 20, 255});

    const int panelW = 560;
    const int panelH = 456;
    const int panelX = (screenW - panelW) / 2;
    const int panelY = (screenH - panelH) / 2;
    Rectangle panel = {
        static_cast<float>(panelX),
        static_cast<float>(panelY),
        static_cast<float>(panelW),
        static_cast<float>(panelH),
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
        const int rowH = 42;
        const int listX = panelX + 44;
        const int listY = panelY + 168;
        const int listW = panelW - 88;
        const int maxVisible = std::min(static_cast<int>(levels.size()), 6);
        for (int i = 0; i < maxVisible; ++i) {
            Rectangle row = {
                static_cast<float>(listX),
                static_cast<float>(listY + i * rowH),
                static_cast<float>(listW),
                static_cast<float>(rowH - 8),
            };
            const bool isSelected = (i == selected);
            DrawRectangleRounded(row, 0.10f, 6,
                                 isSelected ? Color{36, 55, 61, 255}
                                            : Color{31, 37, 48, 230});
            DrawRectangleRoundedLinesEx(row, 0.10f, 6, isSelected ? 2.0f : 1.0f,
                                        isSelected ? Color{109, 236, 218, 255}
                                                   : Color{75, 88, 104, 255});
            DrawTextEx(font, levels[i].name.c_str(),
                       Vector2{static_cast<float>(listX + 18),
                               static_cast<float>(listY + i * rowH + 7)},
                       20.0f, 1.0f, Color{232, 244, 245, 255});
        }
        if (static_cast<int>(levels.size()) > maxVisible) {
            DrawTextEx(font, "More levels available in folder; first 5 shown.",
                       Vector2{static_cast<float>(listX),
                               static_cast<float>(listY + maxVisible * rowH + 6)},
                       14.0f, 1.0f, Color{114, 128, 139, 255});
        }
    }

    DrawTextEx(font, "Enter / Left Click: start    Up/Down: select    N: back to menu from game",
               Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + panelH - 58)},
               15.0f, 1.0f, Color{170, 184, 196, 255});
    if (!message.empty()) {
        DrawTextEx(font, message.c_str(),
                   Vector2{static_cast<float>(panelX + 44), static_cast<float>(panelY + panelH - 30)},
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
    auto resetSoundBaseline = [&]() {
        prevHeld      = game.heldPartIdx;
        prevWon       = game.won;
        prevPlacedCnt = countPlaced(game);
        prevRotateSum = sumRotateCount(game);
    };
    resetSoundBaseline();

    std::vector<LevelEntry> levels = findLevels();
    int selectedLevel = levels.empty() ? -1 : 0;
    std::string menuMessage;

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
                const int hover = menuSelectionFromMouse(levels, sw, sh);
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
                drawMenu(levels, selectedLevel, menuMessage, menuFont, sw, sh);
            } else {
                if (IsKeyPressed(KEY_N)) {
                    appState = AppState::Menu;
                    levels = findLevels();
                    if (selectedLevel < 0 && !levels.empty()) selectedLevel = 0;
                    if (selectedLevel >= static_cast<int>(levels.size())) {
                        selectedLevel = levels.empty() ? -1 : 0;
                    }
                    menuMessage.clear();
                    drawMenu(levels, selectedLevel, menuMessage, menuFont, sw, sh);
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
