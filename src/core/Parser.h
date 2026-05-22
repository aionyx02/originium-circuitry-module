#ifndef ORIGINIUM_CORE_PARSER_H
#define ORIGINIUM_CORE_PARSER_H

#include <istream>
#include <stdexcept>
#include <vector>

#include "Board.h"
#include "Part.h"

struct GameData {
    Board board;
    std::vector<Part> parts;
};

class Parser {
public:
    class Error : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    static GameData parse(std::istream& in);
};

#endif
