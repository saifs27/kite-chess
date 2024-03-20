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
    //REQUIRE(perft(kiwipete, 4) == 4085603);

}

TEST_CASE("position3", "[position3]")
{
    Position pos3("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    REQUIRE(perft(pos3, 1) == 14);
    REQUIRE(perft(pos3, 2) == 191);
    REQUIRE(perft(pos3, 3) == 2812);
    REQUIRE(perft(pos3, 4) == 43238);
    REQUIRE(perft(pos3, 5) == 674624);
}

TEST_CASE("position4", "[position4]")
{
    Position pos4("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    REQUIRE(perft(pos4, 1) == 6);
    REQUIRE(perft(pos4, 2) == 264);
    REQUIRE(perft(pos4, 3) == 9467);
    //REQUIRE(perft(pos4, 4) == 422333);
}

TEST_CASE("position5", "[position5]")
{
    Position pos5("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    REQUIRE(perft(pos5, 1) == 44);
    REQUIRE(perft(pos5, 2) == 1486);
    REQUIRE(perft(pos5, 3) == 62379);
}

TEST_CASE("position6", "[position6]")
{
    Position pos6("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    REQUIRE(perft(pos6, 1) == 46);
    REQUIRE(perft(pos6, 2) == 2079);
    REQUIRE(perft(pos6, 3) == 89890);

}

