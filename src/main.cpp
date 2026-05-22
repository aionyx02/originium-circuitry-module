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

    Renderer renderer;
    while (!WindowShouldClose()) {
        const Action a = Input::poll();
        if (a != Action::None) game.update(a);

        BeginDrawing();
        renderer.draw(game, GetScreenWidth(), GetScreenHeight());
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
