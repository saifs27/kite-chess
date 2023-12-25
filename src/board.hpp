#pragma once
#include "types.hpp"
#include <cstdint>
#include <memory>


class Board { 
    private:
    U64 position; 
    public:
    U64 pieceBitBoard[8];
    int enPassant;


    Board();
    void startPosition();
    U64 getPieceBB(const ColorType color, const PieceType piece);
    void make_move(const Move move);
    void print_position();
};

U64 shift();

U64 attacks();

int population_count(U64 bitboard);

Square msb();
Square pop_lsb();

Square lsb(U64 bitboard);
constexpr U64 fileA_BB = 0x101010101010101ULL;
constexpr U64 fileB_BB = 0x202020202020202ULL;

constexpr U64 fileG_BB = 0x4040404040404040ULL;
constexpr U64 fileH_BB = 0x8080808080808080ULL;


U64 kingAttacks(Board bb,  const ColorType color);
U64 knightAttacks(const Square sq);
U64 pawnAttacks(const Square sq);
U64 bishopAttacks(const Square sq);
U64 rookAttacks(const Square sq);


struct Magic {
    U64 mask;
    U64 magic;
    uint8_t  index_bits;
};

struct XRay {
    Square bishopSquare;
    Square rookSquare;
    U64 rook_attacks(const U64 occupied, const U64 blockers, const Square rookSquare);
    U64 bishop_attacks(const U64 occupied, const U64 blockers, const Square bishopSquare);
    U64 file_attacks(const U64 occupied, const U64 blockers, const Square rookSquare);
    U64 rank_attacks(const U64 occupied, const U64 blockers, const Square rookSquare);
    U64 diagonal_attacks(const U64 occupied, const U64 blockers, const Square bishopSquare);
};



