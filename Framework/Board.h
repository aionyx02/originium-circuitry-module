/**
 * @file Board.h
 * @brief
 */
#ifndef BOARD_H
#define BOARD_H

#include "Part.h"

class Board
{
public:
	unsigned colors;
	unsigned rows;
	unsigned cols;

	enum BoardInfo 
	{
		CANNOT_PLACE = -1,   ///< Cannot place anything
		CANNOT_MOVE = -2,    ///< A part that cannot be moved (-2 - colorIndex)
		OCCUPIED = 1,        ///< A part is placed here (1 + partIndex)
		EMPTY = 0,           ///< Empty
	};

	/// Size: rows * cols
	std::vector<std::vector<BoardInfo>> _boardInfo;

	/// For each rows and columns, the numbers of the grids that should be filled
	/// Size: colors * (rows + cols)
	/// 
	/// _constraints[i] = the numbers of the grids that should be filled for "i-th color"
	std::vector<std::vector<unsigned>> _constraints;
};

#endif
