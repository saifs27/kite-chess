#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include "../src/bitboards.cpp"


using namespace Smyslov;
TEST_CASE("misc", "[misc]") {
    Bitboard bb{set_bit(Square::E4)};
    REQUIRE (try_offset(Square::D4, 1, 1).value() == Square::E5);
    REQUIRE (bb.is_empty() == false);
    REQUIRE (bb.has(Square::E4) == true);
    REQUIRE (bb.has(Square::C4) == false);
}
TEST_CASE("king attacks", "[king]") {
    REQUIRE (king_attacks(set_bit(D4)) == 0x1c141c0000);
    REQUIRE (king_attacks(set_bit(E1)) == 0x3828);
    REQUIRE (king_attacks(set_bit(A1)) == 0x302);
}

TEST_CASE ("knight attacks", "[knight]") {
    REQUIRE (knight_attacks(set_bit(D4)) == 0x142200221400);
    REQUIRE (knight_attacks(set_bit(A1)) == 0x20400);
    REQUIRE (knight_attacks(set_bit(A8)) == 0x4020000000000);
    REQUIRE (knight_attacks(B1) == 0x50800ULL);
    
}

//TEST_CASE ("rook attacks", "[rook]") {
    //REQUIRE (rook_attacks(set_bit(D4)) == 0x8080808f7080808);
//}

TEST_CASE ("pawn attacks", "[pawn]") {
    REQUIRE (pawn_attacks(set_bit(A2)) == 0x20000);
    REQUIRE (pawn_attacks(set_bit(A4)) == 0x200000000);
    REQUIRE (pawn_attacks(set_bit(H2)) == 0x400000);
    REQUIRE (pawn_attacks(set_bit(H4)) == 0x4000000000);
    REQUIRE (pawn_attacks(set_bit(D4)) == 0x1400000000);
}

TEST_CASE("pawn pushes", "[pawnpush]") {
    Move m1(A2, A3, PAWN, WHITE);
    Move m2(A2, A4, PAWN, WHITE);

    Move m3(D7, D5, PAWN, BLACK);
    REQUIRE (pawn_push(m1) == 0x10000);
    REQUIRE (double_pawn_push(m2) == 0x1000000);
    REQUIRE (pawn_push(m3) == 0x80000000000);
    REQUIRE (double_pawn_push(m3) == 0x800000000);
}

