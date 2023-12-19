#pragma once

typedef unsigned long long U64;

enum Column {
    A = 1, B, C, D, E, F, G, H 
};

int convert(Column column, int rank);

typedef unsigned long long U64;
/*
56 57 58 59 60 61 62 63
48 49 50 51 52 53 54 55
40 41 42 43 44 45 46 47
32 33 34 35 36 37 38 39
24 25 26 27 28 29 30 31
16 17 18 19 20 21 22 23
8  9  10 11 12 13 14 15
0  1  2  3  4  5  6  7
*/
class BitBoard {
    
    public:
    U64 pieceBitBoard[8];

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
    void startPosition();
    U64 getPiece(const ColorType color, const PieceType piece);
    U64 kingAttacks(const ColorType color);
    U64 knightAttacks(const ColorType color);
    BitBoard shift();
    




};

