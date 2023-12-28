#include "bitboards.hpp"

U64 set_bit(Square sq) {
    return 0x1ULL << sq;
}

int population_count(U64 bitboard){
    int count = 0;
    while (bitboard) {
        count += bitboard & 0x1ULL;
        bitboard >>= 1;
    }
    return count;
}

Square lsb(U64 bitboard) {
    constexpr int debruijn[32] = {0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
    31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9};
    return (Square)debruijn[((bitboard & -bitboard) * 0x077CB531U) >> 27];
}

Square pop_lsb(U64& bitboard) {
    Square sq = lsb(bitboard);
    bitboard = bitboard - (bitboard & -bitboard);
    return sq;
}

U64 kingAttacks(const U64 bb) {
    U64 kingPosition = bb;
    const U64 left = kingPosition >> 1;
    const U64 right = kingPosition << 1;
    U64 attacks = left | right;

    kingPosition |= attacks;

    const U64 up = kingPosition << 8;
    const U64 down = kingPosition >> 8;

    attacks |= up | down;

    return attacks;
}

U64 knightAttacks(const U64 bb) {
    const U64 notAB = ~fileA_BB & ~fileB_BB;
    const U64 notGH = ~fileG_BB & ~fileH_BB;
    const U64 notA = ~fileA_BB;
    const U64 notH = ~fileH_BB;

    const U64 a = (bb & notH) << 17;
    const U64 b = (bb & notA) << 15;
    const U64 c = (bb & notGH) << 10;
    const U64 d = (bb & notAB) << 6;

    const U64 e = (bb & notA) >> 17;
    const U64 f = (bb & notH) >> 15;
    const U64 g = (bb & notAB) >> 10;
    const U64 h = (bb & notGH) >> 6;

    return a | b | c | d | e | f | g | h ;
}

U64 pawnAttacks(const U64 bb) {
    const U64 notA = ~fileA_BB; 
    const U64 notH = ~fileH_BB;


    const U64 left = (bb & notA) << 7;
    const U64 right = (bb & notH) << 9;

    return left | right;
}

U64 rookAttacks(const U64 bb) {

}

U64 bishopAttacks(const U64 bb) {

}

