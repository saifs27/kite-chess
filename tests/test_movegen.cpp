#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../src/movegen.cpp"

using namespace Kite;

Position opera_game("1n1Rkb1r/p4ppp/4q3/4p1B1/4P3/8/PPP2PPP/2K5 b k - 1 17");
MoveGen opera_moves(opera_game);

Position moscow("rnbqkbnr/pp2pppp/3p4/1Bp5/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 1 3");
MoveGen moscow_moves(moscow);


TEST_CASE("checkmate", "[checkmate]")
{
    REQUIRE (opera_moves.is_checkmate() == true);
    REQUIRE (opera_game.is_check() == true);
    REQUIRE (moscow.is_check() == true);
    //REQUIRE (moscow_moves.is_checkmate() == false);


}