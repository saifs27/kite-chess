#pragma once

#include "movegen.hpp"
#include "types.hpp"


namespace Smyslov {
inline U64 perft(Position& pos, int depth)
{
    std::vector<Move> moveList;
    MoveGen n_moves(pos);
    U64 nodes = 0;
    if (depth == 0)
    {
        return 1ULL;
    }

    n_moves.generate_all_moves();
    
    for (auto i : n_moves.moveList)
    {
        bool isValidMove = n_moves.make_move(i);

        if (!isValidMove) return 0x0ULL;

        nodes += perft(pos, depth - 1);
        n_moves.undo_move();
    }

    return nodes;
}











}