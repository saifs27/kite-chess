#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include "../src/board.cpp"
U64 kd4 = 0x8000000ULL;
Board board;




TEST_CASE("bitboards", "[bitboard]") {
    board.startPosition();
    board.getPieceBB(white, king);
    REQUIRE (board.getPieceBB(white, king) == 0x10ULL);
    REQUIRE (kingAttacks(E1) == 0x3828ULL);
}

TEST_CASE("king on d4", "[king]") {
    REQUIRE (kingAttacks(D4) == 0x1c141c0000);
}

TEST_CASE ("knight attacks", "[knight]") {
    REQUIRE (knightAttacks(D4) == 0x142200221400);
    REQUIRE (knightAttacks(A1) == 0x20400);
    REQUIRE (knightAttacks(A8) == 0x4020000000000);
    
}

TEST_CASE ("rook attacks", "[rook]") {
    REQUIRE (rookAttacks(D4) == 0x8080808f7080808);
}

TEST_CASE ("pawn attacks", "[pawn]") {
    REQUIRE (pawnAttacks(A2) == 0x20000);
    REQUIRE (pawnAttacks(A4) == 0x200000000);
    REQUIRE (pawnAttacks(H2) == 0x400000);
    REQUIRE (pawnAttacks(H4) == 0x4000000000);
    REQUIRE (pawnAttacks(D4) == 0x1400000000);
}

TEST_CASE("make move", "[move]") {
    Move e4;
    e4.from = E2;
    e4.to = E4;
    e4.color = white;
    e4.piece = pawn;
    board.startPosition();
    board.make_move(e4);

    REQUIRE (board.getPieceBB(white, pawn) == 0x1000ef00);
}