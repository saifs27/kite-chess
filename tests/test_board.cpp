#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include "../src/bitboards.cpp"




TEST_CASE("king attacks", "[king]") {
    REQUIRE (kingAttacks(set_bit(D4)) == 0x1c141c0000);
    REQUIRE (kingAttacks(set_bit(E1)) == 0x3828);
    REQUIRE (kingAttacks(set_bit(A1)) == 0x302);
}

TEST_CASE ("knight attacks", "[knight]") {
    REQUIRE (knightAttacks(set_bit(D4)) == 0x142200221400);
    REQUIRE (knightAttacks(set_bit(A1)) == 0x20400);
    REQUIRE (knightAttacks(set_bit(A8)) == 0x4020000000000);
    
}

//TEST_CASE ("rook attacks", "[rook]") {
    //REQUIRE (rookAttacks(set_bit(D4)) == 0x8080808f7080808);
//}

TEST_CASE ("pawn attacks", "[pawn]") {
    REQUIRE (pawnAttacks(set_bit(A2)) == 0x20000);
    REQUIRE (pawnAttacks(set_bit(A4)) == 0x200000000);
    REQUIRE (pawnAttacks(set_bit(H2)) == 0x400000);
    REQUIRE (pawnAttacks(set_bit(H4)) == 0x4000000000);
    REQUIRE (pawnAttacks(set_bit(D4)) == 0x1400000000);
}

