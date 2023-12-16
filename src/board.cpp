#include "board.hpp"

int convert(Column column, int row) {
    return row * 8 + column;
}

BitBoard::BitBoard() {
    white = 0xffff;
    black = 0xffff000000000000;
    pawn = 0xff00000000ff00;
    knight = 0x4200000000000042;
    bishop = 0x2400000000000024;
    rook = 0x8100000000000081;
    queen = 0x800000000000008;
    king = 0x1000000000000010;   
}