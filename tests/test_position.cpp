#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/position.cpp"

using namespace Kite;
Position board;


TEST_CASE("uci", "[uci]") {
    board.start_position();
    Move move(Square::E2, Square::E4, Flag::DOUBLE_PAWN);

    REQUIRE (move.from() == Square::E2);
    REQUIRE (move.to() == Square::E4);
    REQUIRE (move.flags() == Flag::DOUBLE_PAWN);

}


TEST_CASE("make move", "[move]") {
    board.start_position();
    //board.make_move(Move(Square::E2, Square::E4, Flag::DOUBLE_PAWN));
    // TODO add double pawn pushes
    //REQUIRE (board.get_bitboard(WHITE, PAWN) == 0x1000ef00);
}

TEST_CASE("population count", "[popcount]") {
    board.start_position();
    auto whitepawn {board.get_bitboard(Color::WHITE, Piece::PAWN)};
    REQUIRE (Bitboard::population_count(whitepawn) == 8);
}

TEST_CASE("bitboards", "[bitboard]") {
    board.start_position();
    REQUIRE (board.get_bitboard(Color::WHITE, Piece::KING) == 0x10ULL);
}

TEST_CASE("misc board", "[misc board]") {
    board.start_position();
    REQUIRE (board.check_square_color(Square::A1) == Color::WHITE);
    REQUIRE (board.check_square_color(Square::D7) == Color::BLACK);
    REQUIRE (board.get_piece(Square::D1) == Piece::QUEEN);

}

TEST_CASE("check mask" "[check mask]")
{
    Position pos("8/5rpk/4Q3/P1p1p1p1/4p3/4B3/3R1PP1/q5K1 w - - 3 50");
    Position queenCheck("Q3k3/8/8/8/8/8/8/6K1 w - - 0 1");
    REQUIRE (pos.check_mask(Color::WHITE) == 0x3f);
    REQUIRE (queenCheck.check_mask(Color::BLACK) == 0xf00000000000000);
}