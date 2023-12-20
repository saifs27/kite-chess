#include "board.hpp"

BitBoard::BitBoard() {
    pieceBitBoard[white] = 0xffff;
    pieceBitBoard[black] = 0xffff000000000000;
    pieceBitBoard[pawn] = 0xff00000000ff00;
    pieceBitBoard[knight] = 0x4200000000000042;
    pieceBitBoard[bishop] = 0x2400000000000024;
    pieceBitBoard[rook] = 0x8100000000000081;
    pieceBitBoard[queen] = 0x800000000000008;
    pieceBitBoard[king] = 0x1000000000000010;
}

U64 BitBoard::getPiece(const ColorType color, const PieceType piece) {
    return pieceBitBoard[color] & pieceBitBoard[piece];
}

U64 BitBoard::kingAttacks(const ColorType color) {
    U64 kingPosition = getPiece(color, king);
    const U64 left = kingPosition >> 1;
    const U64 right = kingPosition << 1;
    U64 attacks = left | right;

    kingPosition |= attacks;

    const U64 up = kingPosition << 8;
    const U64 down = kingPosition >> 8;

    attacks |= up | down;

    return attacks;
}

U64 knightAttacks(Square sq) {
    U64 bit = 0x1;

    U64 a = (sq + 17 < 64) ? bit << (sq + 17) : 0x0;
    U64 b = (sq + 15 < 64) ? bit << (sq + 15) : 0x0;
    U64 c = (sq + 10 < 64) ? bit << (sq + 10) : 0x0;
    U64 d = (sq + 6 < 64)  ? bit << (sq + 6) : 0x0;

    U64 e = (sq > 17) ? bit << (sq - 17) : 0x0;
    U64 f = (sq > 15) ? bit << (sq - 15) : 0x0;
    U64 g = (sq > 10) ? bit << (sq - 10) : 0x0;
    U64 h = (sq > 6)  ? bit << (sq - 6) : 0x0;

    return a | b | c | d | e | f | g | h ;

}

