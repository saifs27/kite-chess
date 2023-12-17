#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include "../src/board.cpp"

TEST_CASE("bitboards", "[BitBoard]") {
    U64 kd4 = 0x8000000;
    BitBoard board;
    board.getPiece(board.white, board.king);
    REQUIRE (board.getPiece(board.white, board.king) == 0x10);
    REQUIRE (board.kingAttacks() == 0x3828);
}