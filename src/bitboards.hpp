#pragma once
#include "types.hpp"
#include <cstdint>
#include <memory>
#include <vector>

U64 set_bit(Square sq);

int population_count(U64 bitboard);

Square msb(U64 bitboard);
Square lsb(U64 bitboard);
Square pop_lsb(U64& bitboard);

constexpr U64 fileA_BB = 0x101010101010101ULL;
constexpr U64 fileB_BB = 0x202020202020202ULL;

constexpr U64 fileG_BB = 0x4040404040404040ULL;
constexpr U64 fileH_BB = 0x8080808080808080ULL;


U64 kingAttacks(const U64 bb);
U64 knightAttacks(const U64 bb);
U64 pawnAttacks(const U64 bb);
U64 bishopAttacks(const U64 bb);
U64 rookAttacks(const U64 bb);


struct Magic {
    U64 mask;
    U64 magic;
    U64& attacks;
    uint8_t  index_bits;
};





