#include "board.hpp"
BitBoard::BitBoard() {
    pieceBitBoard[white] = 0x0ULL;
    pieceBitBoard[black] = 0x0ULL;
    pieceBitBoard[pawn] = 0x0ULL;
    pieceBitBoard[knight] = 0x0ULL;
    pieceBitBoard[bishop] = 0x0ULL;
    pieceBitBoard[rook] = 0x0ULL;
    pieceBitBoard[queen] = 0x0ULL;
    pieceBitBoard[king] = 0x0ULL;
}
void BitBoard::startPosition() {
    pieceBitBoard[white] = 0xffff;
    pieceBitBoard[black] = 0xffff000000000000;
    pieceBitBoard[pawn] = 0xff00000000ff00;
    pieceBitBoard[knight] = 0x4200000000000042;
    pieceBitBoard[bishop] = 0x2400000000000024;
    pieceBitBoard[rook] = 0x8100000000000081;
    pieceBitBoard[queen] = 0x800000000000008;
    pieceBitBoard[king] = 0x1000000000000010;
}

U64 BitBoard::getPieceBB(const ColorType color, const PieceType piece) {
    return pieceBitBoard[color] & pieceBitBoard[piece];
}

U64 kingAttacks(BitBoard &bb, const ColorType color) {
    U64 kingPosition = bb.getPieceBB(color, king);
    const U64 left = kingPosition >> 1;
    const U64 right = kingPosition << 1;
    U64 attacks = left | right;

    kingPosition |= attacks;

    const U64 up = kingPosition << 8;
    const U64 down = kingPosition >> 8;

    attacks |= up | down;

    return attacks;
}

U64 knightAttacks(const Square sq) {
    const U64 bit = 0x1ULL << sq;
    const U64 notAB = ~fileA_BB & ~fileB_BB;
    const U64 notGH = ~fileG_BB & ~fileH_BB;
    const U64 notA = ~fileA_BB;
    const U64 notH = ~fileH_BB;



    const U64 a = (bit & notH) << 17;
    const U64 b = (bit & notA) << 15;
    const U64 c = (bit & notGH) << 10;
    const U64 d = (bit & notAB) << 6;

    const U64 e = (bit & notA) >> 17;
    const U64 f = (bit & notH) >> 15;
    const U64 g = (bit & notAB) >> 10;
    const U64 h = (bit & notGH) >> 6;

    return a | b | c | d | e | f | g | h ;

}

U64 rookAttacks(const Square sq) {
    const int column = sq % 8;
    const int rank = sq / 8;
    U64 bit = 0x1ULL;
    U64 board = 0x0ULL;


    for (int i = 1; i < 8 - column; i++) {
        board |= (bit << (sq + 8*i));
    }
    for (int i = 1; i < 8 - rank; i++) {
        board |= (bit << (sq + i));
    }
    for (int i = 1; i <= column; i++) {
        board |= (bit << (sq - 8*i));
    }
    for (int i = 1; i <= rank; i++) {
        board |= (bit << (sq - i));
    }

    return board;
}

