#pragma once
#include <algorithm>

typedef unsigned long long U64;

enum Square {
    A1, A2, A3, A4, A5, A6, A7, A8,
    B1, B2, B3, B4, B5, B6, B7, B8,
    C1, C2, C3, C4, C5, C6, C7, C8,
    D1, D2, D3, D4, D5, D6, D7, D8,
    E1, E2, E3, E4, E5, E6, E7, E8,
    F1, F2, F3, F4, F5, F6, F7, F8,
    G1, G2, G3, G4, G5, G6, G7, G8,
    H1, H2, H3, H4, H5, H6, H7, H8,

};

typedef unsigned long long U64;

enum ColorType {white, black};
enum PieceType {pawn = 2, knight, bishop, rook, queen, king};

class BitBoard {
    
    public:
    U64 pieceBitBoard[8];
    BitBoard();
    void startPosition();
    U64 getPiece(const ColorType color, const PieceType piece);
    U64 kingAttacks(const ColorType color);

    U64 shift();

    U64 wSinglePush();

    U64 attacks();

    int population_count();
    




};

U64 knightAttacks(Square sq);