#pragma once
#include "types.hpp"
#include "rays.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace Smyslov {

bool is_empty(U64 bb);
bool has(U64 bb, Square sq);


int population_count(const U64 bitboard);
Square msb(const U64 bitboard);
Square lsb(const U64 bitboard);
Square pop_lsb(U64& bitboard);
void print_bitboard(const U64 bitboard);
std::vector<Square> get_squares(const U64 bb);




U64 king_attacksBB(const U64 bb);
U64 knight_attacksBB(const U64 bb);
U64 pawn_attacksBB(const U64 bb, const Color color);
U64 bishop_attacks(const Square sq, const U64 blockers);
U64 rook_attacks(const Square sq, const U64 blockers);

U64 pawn_pushBB(const U64 bb, const Color color );
U64 double_pawn_pushBB(const U64 bb, const Color color);


inline U64 king_attacks_sq(const Square sq) 
{
    U64 bb = set_bit(sq);
    return king_attacksBB(bb);
}

inline U64 knight_attacks_sq(const Square sq)
{
    U64 bb = set_bit(sq);
    return knight_attacksBB(bb);
}

inline U64 pawn_attacks_sq(const Square sq, const Color color)
{
    U64 bb = set_bit(sq);
    return pawn_attacksBB(bb, color);
}



inline U64 pawn_push_sq(const Square sq, const Color color)
{
    U64 bb = set_bit(sq);
    return pawn_pushBB(bb, color);
}

inline U64 double_pawn_push_sq(const Square sq, const Color color)
{
    U64 bb = set_bit(sq);
    return double_pawn_pushBB(bb, color);
}




}




