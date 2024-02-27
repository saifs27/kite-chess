#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include "../src/bitboards.cpp"
#include "../src/moves.cpp"


using namespace Smyslov;
TEST_CASE("misc", "[misc]") {
    U64 bb = set_bit(Square::E4);
    REQUIRE (try_offset(Square::D4, 1, 1).value() == Square::E5);
    REQUIRE (is_empty(bb) == false);
    REQUIRE (has(bb, Square::E4) == true);
    REQUIRE (has(bb, Square::C4) == false);
}
TEST_CASE("king attacks", "[king]") {
    REQUIRE (king_attacks(set_bit(Square::D4)) == 0x1c141c0000);
    REQUIRE (king_attacks(set_bit(Square::E1)) == 0x3828);
    REQUIRE (king_attacks(set_bit(Square::A1)) == 0x302);
}

TEST_CASE ("knight attacks", "[knight]") {
    REQUIRE (knight_attacks(set_bit(Square::D4)) == 0x142200221400);
    REQUIRE (knight_attacks(set_bit(Square::A1)) == 0x20400);
    REQUIRE (knight_attacks(set_bit(Square::A8)) == 0x4020000000000);
    REQUIRE (knight_attacks(set_bit(Square::B1)) == 0x50800ULL);
    
}

//TEST_CASE ("rook attacks", "[rook]") {
    //REQUIRE (rook_attacks(set_bit(D4)) == 0x8080808f7080808);
//}

TEST_CASE ("pawn attacks", "[pawn]") {
    REQUIRE (pawn_attacks_sq(Square::A2, Color::WHITE) == 0x20000);
    REQUIRE (pawn_attacks(set_bit(Square::A4), Color::WHITE) == 0x200000000);
    REQUIRE (pawn_attacks(set_bit(Square::H2), Color::WHITE) == 0x400000);
    REQUIRE (pawn_attacks(set_bit(Square::H4), Color::WHITE) == 0x4000000000);
    REQUIRE (pawn_attacks(set_bit(Square::D4), Color::WHITE) == 0x1400000000);
}

TEST_CASE("pawn pushes", "[pawnpush]") {
    Move m1(Square::A2, Square::A3, Flag::QUIET);
    Move m2(Square::A2, Square::A4, Flag::QUIET);

    Move m3(Square::D7, Square::D5, Flag::QUIET);
    REQUIRE (pawn_push_sq(m1.from(), Color::WHITE) == 0x10000);
    REQUIRE (double_pawn_push_sq(m2.from(), Color::WHITE) == 0x1000000);
    REQUIRE (pawn_push_sq(m3.from(), Color::BLACK) == 0x80000000000);
    REQUIRE (double_pawn_push_sq(m3.from(), Color::BLACK) == 0x800000000);
}

TEST_CASE ("bishop attacks", "[bishop]") {
    REQUIRE (bishop_attacks(Square::D4, 0x0ULL) == 0x8041221400142241);
    REQUIRE (Rays::getRayAttacks(Square::D4, Rays::Direction::NE) == 0x8040201000000000);
}

TEST_CASE ("msb", "[msb]") {
    REQUIRE (msb(set_bit(Square::D4)) == Square::D4);
}






