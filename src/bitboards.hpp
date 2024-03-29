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
#include "types.hpp"
#include "rays.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "moves.hpp"
namespace Kite::Bitboard {

int population_count(const U64 bitboard);
Square msb(const U64 bitboard);
Square lsb(const U64 bitboard);
Square pop_lsb(U64& bitboard);
void print_bitboard(const U64 bitboard);
std::vector<Square> get_squares(const U64 bb);

U64 king_attacks(const U64 bb);
U64 knight_attacks(const U64 bb);
U64 pawn_attacks(const U64 bb, const Color color);
U64 bishop_attacks(const Square sq, const U64 blockers);
U64 rook_attacks(const Square sq, const U64 blockers);

U64 pawn_push(const U64 bb, const Color color );
U64 double_pawn_push(const U64 bb, const Color color);


inline U64 king_attacks_sq(const Square sq) 
{
    U64 bb = set_bit(sq);
    return king_attacks(bb);
}

inline U64 knight_attacks_sq(const Square sq)
{
    U64 bb = set_bit(sq);
    return knight_attacks(bb);
}

inline U64 pawn_attacks_sq(const Square sq, const Color color)
{
    U64 bb = set_bit(sq);
    return pawn_attacks(bb, color);
}



inline U64 pawn_push_sq(const Square sq, const Color color)
{
    U64 bb = set_bit(sq);
    return pawn_push(bb, color);
}

inline U64 double_pawn_push_sq(const Square sq, const Color color)
{
    U64 bb = set_bit(sq);
    return double_pawn_push(bb, color);
}




}




