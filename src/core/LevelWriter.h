#ifndef ORIGINIUM_CORE_LEVELWRITER_H
#define ORIGINIUM_CORE_LEVELWRITER_H

#include <string>
#include <vector>

#include "Board.h"
#include "Part.h"

// Serialize a Board + parts back into the plain-text level format — the exact
// inverse of Parser::parse (see docs/io/input-file-format.md). Pure core, no
// raylib. Used by the level editor's export. Round-trips with Parser: parsing
// the output reproduces an equivalent Board + parts.
class LevelWriter {
public:
    static std::string write(const Board& board, const std::vector<Part>& parts);
};

#endif
