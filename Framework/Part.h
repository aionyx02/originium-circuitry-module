/**
 * @file Part.h
 * @brief
 */
#ifndef PART_H
#define PART_H

#include <vector>

enum class Rotate
{
	CW_0 = 0,     // Rotate Clock-wise 0 degree
	CW_90 = 1,    // 90 degrees
	CW_180 = 2,   // 180 degrees
	CW_270 = 3,   // 270 degrees
};

struct PartLocation {
    int row;
    int col;
    Rotate rotate;
    bool placed; // Is it placed on the board

    PartLocation() : row(0), col(0), rotate(Rotate::CW_0), placed(false) {}
};

class Part
{
public:
	unsigned colorIndex;        /// The index of this part's color
	unsigned partIndex;         /// The index of this part
	std::vector<std::vector<bool>> shape; /// How this part looks like，False -> Empty; True -> Ocuppied
	PartLocation location;      /// Where is the part on the board?
};

#endif
