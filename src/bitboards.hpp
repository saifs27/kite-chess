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






