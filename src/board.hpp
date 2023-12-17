#pragma once

#include <cstdint>

typedef unsigned long long U64;

enum Column {
    A = 1, B, C, D, E, F, G, H 
};

int convert(Column column, int rank);

typedef unsigned long long U64;

class BitBoard {
    U64 pieceBitBoard[8];
    public:
    enum ColorType {white, black};

    enum PieceType {
    pawn = 2,
    knight,
    bishop,
    rook,
    queen,
    king,
    };


    BitBoard();
    U64 getPiece(const ColorType color, const PieceType piece);
    U64 kingAttacks();
    




};

