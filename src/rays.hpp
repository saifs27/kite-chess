#pragma once
#include "types.hpp"
namespace Smyslov {


namespace Rays
{
    enum Direction {NORTH, SOUTH, EAST, WEST, NE, NW, SE, SW};
    inline U64 rayAttacks[64][8];
    void init();
    inline U64 getRayAttacks(Square sq, Direction dir)
    {
        return rayAttacks[static_cast<int>(sq)][static_cast<int>(dir)];
    }

}
}