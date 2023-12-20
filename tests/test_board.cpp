#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include "../src/board.cpp"
U64 kd4 = 0x8000000;
BitBoard board;


TEST_CASE("bitboards", "[bitboard]") {

    board.getPiece(white, king);
    REQUIRE (board.getPiece(white, king) == 0x10);
    REQUIRE (board.kingAttacks(white) == 0x3828);
}

TEST_CASE("king on d4", "[king]") {
    board.pieceBitBoard[king] = 0x1000000008000000;  
    board.pieceBitBoard[white] = 0x800ffef;
    REQUIRE (board.kingAttacks(white) == 0x1c141c0000);
}

TEST_CASE ("knight attacks", "[knight]") {
    REQUIRE (knightAttacks(D4) == 0x142200221400);
}
