#include "board.hpp"

int convert(Column column, int row) {
    return row * 8 + column;
}

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

U64 BitBoard::kingAttacks() {
    U64 kingPosition = getPiece(white, king);
    const U64 left = kingPosition >> 1;
    const U64 right = kingPosition << 1;
    U64 attacks = left | right;

    kingPosition |= attacks;

    const U64 up = kingPosition << 8;
    const U64 down = kingPosition >> 8;

    attacks |= up | down;

    return attacks;
}