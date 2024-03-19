#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/perft.hpp"

using namespace Smyslov;


TEST_CASE("startpos", "[startpos]")
{
    Position startpos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    REQUIRE(perft(startpos, 1) == 20);
    REQUIRE(perft(startpos, 2) == 400);
    REQUIRE(perft(startpos, 3) == 8902);
    REQUIRE(perft(startpos, 4) == 197281);
}

TEST_CASE("kiwipete", "[kiwipete]")
{
    Position kiwipete("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    REQUIRE(perft(kiwipete, 1) == 48);
    REQUIRE(perft(kiwipete, 2) == 2039);
    REQUIRE(perft(kiwipete, 3) == 97862);
    REQUIRE(perft(kiwipete, 4) == 4085603);

}