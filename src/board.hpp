#pragma once

#include <cstdint>

typedef unsigned long long U64;

enum Column {
    A = 1, B, C, D, E, F, G, H 
};

int convert(Column column, int rank);

typedef unsigned long long U64;

class BitBoard {
    public:
    U64 white;
    U64 black;
    U64 pawn;
    U64 knight;
    U64 bishop;
    U64 rook;
    U64 queen;
    U64 king;



};

