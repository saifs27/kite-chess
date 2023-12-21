#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include "../src/board.cpp"
U64 kd4 = 0x8000000ULL;
    BitBoard board;



TEST_CASE("bitboards", "[bitboard]") {
    board.startPosition();
    board.getPieceBB(white, king);
    REQUIRE (board.getPieceBB(white, king) == 0x10ULL);
    REQUIRE (kingAttacks(board, white) == 0x3828ULL);
}

TEST_CASE("king on d4", "[king]") {
    board.pieceBitBoard[king] = 0x1000000008000000;  
    board.pieceBitBoard[white] = 0x800ffef;
    REQUIRE (kingAttacks(board, white) == 0x1c141c0000);
}

TEST_CASE ("knight attacks", "[knight]") {
    REQUIRE (knightAttacks(D4) == 0x142200221400);
}

TEST_CASE ("rook attacks", "[rook]") {
    REQUIRE (rookAttacks(D4) == 0x8080808f7080808);
}
