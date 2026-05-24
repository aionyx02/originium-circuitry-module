#include <fstream>
#include <iostream>
#include <string>

#include "raylib.h"

#include "core/Game.h"
#include "core/Parser.h"
#include "ui/Input.h"
#include "ui/Renderer.h"

namespace {

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
    std::string path = promptPath(argc, argv);
    if (path.empty()) {
        std::cerr << "No level file given." << std::endl;
        return 1;
    }

    Game game;
    std::string err;
    if (!loadGame(path, game, err)) {
        std::cerr << err << std::endl;
        return 1;
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

    int  prevHeld       = game.heldPartIdx;
    bool prevWon        = game.won;
    int  prevPlacedCnt  = countPlaced(game);
    int  prevRotateSum  = sumRotateCount(game);

    {
        Renderer renderer;
        while (!WindowShouldClose()) {
            const int sw = GetScreenWidth();
            const int sh = GetScreenHeight();
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

            BeginDrawing();
            renderer.draw(game, sw, sh, GetFrameTime());
            EndDrawing();
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
