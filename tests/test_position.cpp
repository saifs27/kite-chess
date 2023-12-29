#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/position.cpp"

Position board;

TEST_CASE("uci", "[uci]") {
    board.startPosition();
    auto move = board.uci_to_move("e2e4");
    REQUIRE (move.color == WHITE);
    REQUIRE (move.piece == PAWN);    
    REQUIRE (move.from == E2);
    REQUIRE (move.to == E4);
    REQUIRE (move.moveType == NORMAL);

    REQUIRE (move.promoted == EMPTY);
}

//TEST_CASE("make move", "[move]") {
//    Move e4;
 //   e4.from = E2;
 //   e4.to = E4;
 //   e4.color = WHITE;
//    e4.piece = PAWN;
 //   board.make_move("e2e4");
//
 //   REQUIRE (board.getPieceBB(WHITE, PAWN) == 0x1000ef00);
//}

TEST_CASE("population count", "[popcount]") {
    board.startPosition();
    U64 whitepawn = board.getPieceBB(WHITE, PAWN);
    REQUIRE (population_count(whitepawn) == 8);
}

TEST_CASE("bitboards", "[bitboard]") {
    board.startPosition();
    REQUIRE (board.getPieceBB(WHITE, KING) == 0x10ULL);
}