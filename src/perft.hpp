#pragma once

#include "movegen.hpp"
#include "types.hpp"


namespace Smyslov {
inline U64 perft(Position& pos, int depth)
{
    std::vector<Move> moveList;
    MoveGen generate{moveList, pos};
    U64 nodes = 0;
    if (depth == 0)
    {
        return 1ULL;
    }

    generate.generate_all_moves();
    for (auto i : generate.moveList)
    {
        pos.make_move(i);
        nodes += perft(pos, depth - 1);
        pos.undo_move();
    }

    return nodes;
}











}