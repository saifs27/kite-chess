#pragma once
#include "types.hpp"
#include "position.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <random>
#include <tuple>
#include <optional>

class Bitboard {
    public:
    U64 bitboard;
    template <typename T>
    Bitboard operator|(T bb);
    void operator|=(Bitboard bb);
    Bitboard operator&(Bitboard bb);
    void operator&=(Bitboard bb);
    Bitboard operator^(Bitboard bb);
    void operator^=(Bitboard bb);
    Bitboard operator~();
    Bitboard operator<<(int n);
    void operator<<=(int n);
    Bitboard operator>>(int n);
    void operator>>=(int n);
    template <typename T>
    void operator=(T bitboard);
    template <typename T>  
    bool operator==(T bb);
    template <typename T>
    bool operator!=(T bb);

    bool is_empty();
    bool has(Square sq);
    int population_count();
    Square msb();
    Square lsb();
    Square pop_lsb();
    void print_bitboard();

};

U64 set_bit(Square sq);


U64 king_attacks(const U64 bb);
U64 knight_attacks(const U64 bb);
U64 pawn_attacks(const U64 bb);
U64 bishop_attacks(const U64 bb);
U64 rook_attacks(const U64 bb);

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





