/*
    Kite, a UCI compliant chess engine.
    Copyright (C) 2024  Saif

    Kite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Kite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#pragma once

#include <array>

#include "types.hpp"
namespace Kite::Rays {
enum Direction { NORTH, SOUTH, EAST, WEST, NE, NW, SE, SW };

/*
Initializing rayAttacks at compile time using lambdas.
The () at the end is to call the lambda right away.
*/

inline static constexpr auto rayAttacks = []() {
    std::array<std::array<U64, 8>, 64> rays{};
    U64 north = 0x0101010101010100ULL;
    U64 south = 0x80808080808080;
    U64 west = 0x7f00000000000000;
    U64 east = 0xfe;
    U64 ne = 0x8040201008040200ULL;
    U64 nw = 0x102040810204000ULL;
    U64 sw = 0x40201008040201;
    U64 se = 0x2040810204080;
    for (int sq = 0; sq < 64; sq++, north <<= 1, south >>= 1) {
        rays[sq][static_cast<int>(Direction::NORTH)] = north;
        rays[63 - sq][static_cast<int>(Direction::SOUTH)] = south;
    }

    for (int sq = 0; sq < 64; sq++, east <<= 1, west >>= 1) {
        rays[sq][static_cast<int>(Direction::EAST)] =
            east & get_rank_mask<int>(sq);
        rays[63 - sq][static_cast<int>(Direction::WEST)] =
            west & get_rank_mask<int>(63 - sq);
    }

    for (int f = 0; f < 8; f++, ne <<= 1, nw >>= 1) {
        U64 northeast = ne;
        U64 northwest = nw;
        for (int r = 0; r < 64; r += 8, northeast <<= 8, northwest <<= 8) {
            rays[r + f][static_cast<int>(Direction::NE)] =
                northeast & mask_east(r + f);
            rays[r + (7 - f)][static_cast<int>(Direction::NW)] =
                northwest & mask_west(r + (7 - f));
        }
    }

    for (int f = 0; f < 8; f++, se <<= 1, sw >>= 1) {
        U64 southeast = se;
        U64 southwest = sw;
        for (int r = 56; r >= 0; r -= 8, southeast >>= 8, southwest >>= 8) {
            rays[r + f][static_cast<int>(Direction::SE)] =
                southeast & mask_east(r + f);
            rays[r + (7 - f)][static_cast<int>(Direction::SW)] =
                southwest & mask_west(r + (7 - f));
        }
    }

    return rays;
}();

inline U64 get_ray_attacks(Square sq, Direction dir) {
    if (sq == Square::EMPTY_SQUARE) return 0x0ULL;
    return rayAttacks[static_cast<int>(sq)][static_cast<int>(dir)];
}

}  // namespace Kite::Rays