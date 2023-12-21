#pragma once
#include "types.hpp"
#include <cstdint>

class BitBoard {  
    public:
    U64 pieceBitBoard[8];

    BitBoard();
    void startPosition();
    U64 getPieceBB(const ColorType color, const PieceType piece);

    U64 shift();

    U64 attacks();

    int population_count();
    Square lsb();
    Square msb();
    Square pop_lsb();
};

const U64 fileA_BB = 0x101010101010101ULL;
const U64 fileB_BB = 0x202020202020202ULL;

const U64 fileG_BB = 0x4040404040404040ULL;
const U64 fileH_BB = 0x8080808080808080ULL;


U64 kingAttacks(BitBoard &bb,  const ColorType color);
U64 knightAttacks(const Square sq);
U64 pawnAttacks(const ColorType color, const Square sq);
U64 bishopAttacks(const Square sq);
U64 rookAttacks(const Square sq);


struct Magic {
    BitBoard mask;
    U64 magic;
    uint8_t  index_bits;
};

