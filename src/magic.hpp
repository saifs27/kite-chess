#pragma once
#include "bitboards.hpp"
struct Slider {
    std::tuple<int,int> deltas[4];
    Bitboard moves(const Square square, Bitboard blockers);
    Bitboard get_relevant_blockers(const Square sq);
};

namespace slider {
inline constexpr Slider ROOK {std::make_tuple(1,0), std::make_tuple(0,-1), std::make_tuple(-1,0), std::make_tuple(0,1)};
inline constexpr Slider BISHOP {std::make_tuple(1,1), std::make_tuple(1,-1), std::make_tuple(-1,-1), std::make_tuple(-1,1)};
}

struct Magic {
    U64 mask;
    U64 magic;
    U64 shift;
    U64 offset;
};

//Magic ROOK_MAGICS[64];
//Magic BISHOP_MAGICS[64];

//U64 ROOK_MOVES[64];
 //U64 BISHOP_MOVES[64];

U64 magic_index(const Magic& entry, const U64 blockers);
void find_magic(Slider& slider, Square square, u_int8_t index_bits);
U64 get_rook_moves(Square sq, U64 blockers);
U64 get_bishop_moves(Square sq, U64 blockers);
