#pragma once
#include "types.hpp"
#include "rays.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <random>
#include <optional>
#include <variant>

namespace Smyslov {

bool is_empty(U64 bb);
bool has(U64 bb, Square sq);


int population_count(const U64 bitboard);
Square msb(const U64 bitboard);
Square lsb(const U64 bitboard);
Square pop_lsb(U64& bitboard);
void print_bitboard(const U64 bitboard);





U64 king_attacks(const U64 bb);
U64 knight_attacks(const U64 bb);
U64 pawn_attacks(const U64 bb);
U64 bishop_attacks(const Square sq, const U64 blockers);
U64 rook_attacks(const Square sq, const U64 blockers);

U64 pawn_push(Color color, const U64 bb);
U64 double_pawn_push(Color color, const U64 bb);



inline U64 king_attacks(const Square sq) 
{
    U64 bb = set_bit(sq);
    return king_attacks(bb);
}

inline U64 knight_attacks(const Square sq)
{
    U64 bb = set_bit(sq);
    return knight_attacks(bb);
}

inline U64 pawn_attacks(const Square sq)
{
    U64 bb = set_bit(sq);
    return pawn_attacks(bb);
}



inline U64 pawn_push(const Square sq, const Color color)
{
    U64 bb = set_bit(sq);
    return pawn_push(color, bb);
}

inline U64 double_pawn_push(const Square sq, const Color color)
{
    U64 bb = set_bit(sq);
    return double_pawn_push(color, bb);
}




}




