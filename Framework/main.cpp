#include <iostream>
#include "Board.h"
#include "Part.h"

class Game {
    std::vector<Part> parts;
    Board board;

public:
    /**
     * Read the input stream and intialize the `board` and `parts`
     */
    void init(std::istream& in) {}

    /**
     * Start the gameloop
     */
    void gameloop() {}
};

int main() {
    // change terminal's code page to UTF-8
    system("chcp 65001 && cls");
    
    Game game;
    // game.init( ... );
    game.gameloop();
}
