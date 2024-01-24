#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/position.cpp"

using namespace Smyslov;
Position board;

TEST_CASE("uci", "[uci]") {
    board.start_position();
    auto move = board.uci_to_move("e2e4").value();
    move.set_flags(Flag::NOFLAG);
    REQUIRE (move.from() == Square::E2);
    REQUIRE (move.to() == Square::E4);
    REQUIRE (move.flags() == Flag::NOFLAG);

}

TEST_CASE("legal" "[legal]") {
    board.start_position();
    Move m(Square::G1, Square::G3, Flag::NOFLAG);
    REQUIRE(board.is_pseudo_legal(m) == false);
}

TEST_CASE("make move", "[move]") {
    board.start_position();
    board.make_move("e2e4");
    // TODO add double pawn pushes
    //REQUIRE (board.get_bitboard(WHITE, PAWN) == 0x1000ef00);
}

TEST_CASE("population count", "[popcount]") {
    board.start_position();
    auto whitepawn {board.get_bitboard(Color::WHITE, Piece::PAWN)};
    REQUIRE (population_count(whitepawn) == 8);
}

TEST_CASE("bitboards", "[bitboard]") {
    board.start_position();
    REQUIRE (board.get_bitboard(Color::WHITE, Piece::KING) == 0x10ULL);
}