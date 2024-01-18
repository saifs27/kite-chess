#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/magic.cpp"

using namespace Smyslov;

TEST_CASE("slider", "[slider]") {
    Slider rook_a{slider::rook};
    Slider bishop_a{slider::bishop};
    auto rook_relevant_blockers = rook_a.get_relevant_blockers(Smyslov::Square::A1);

    REQUIRE (rook_a.moves(Square::D4, Bitboard{0x0ULL}).bitboard == 0x8080808f7080808);
    REQUIRE (rook_a.moves(Square::A1, Bitboard{0x10000000020}).bitboard == 0x1010101013e);

    REQUIRE (bishop_a.moves(Square::D4, Bitboard{0x0ULL}).bitboard == 0x8041221400142241);
    REQUIRE (bishop_a.moves(Square::D4, Bitboard{0x40020000100000}).bitboard == 0x40221400140201);
    REQUIRE (rook_relevant_blockers == 282578800148862);

}