#pragma once

#include "movegen.hpp"
#include "types.hpp"
#include <algorithm>
#include <vector>
#include <string>
#include "uci.hpp"
namespace Smyslov {



struct Output
{
    std::string move;
    U64 nodes;
};

bool compareOutputs(Output a, Output b) {return a.move < b.move;}


inline std::vector<Output> perft(Position& pos, int depth)
{
    std::vector<Output> result;
    std::vector<Move> moveList;
    MoveGen n_moves(pos);
    U64 nodes = 0;
    if (depth == 0)
    {
        return {{"null", 1ULL}};
    }

    n_moves.generate_all_moves();
    
    for (auto i : n_moves.moveList)
    {   
        std::string move_string = UCI::move_to_uci(i);
        U64 move_nodes = 0;
        bool isValidMove = n_moves.make_move(i);

        if (!isValidMove) return {{move_string, 0x0ULL}};

        //nodes += perft(pos, depth - 1);
        move_nodes += perft(pos, depth -1).back().nodes;
        nodes += move_nodes;
        Output output = {move_string, move_nodes};
        result.push_back(output);
        n_moves.undo_move();
    }
    result.push_back({"total", nodes});
    return result;
}


inline void print_perft(Position& pos, short depth)
{
    auto result = perft(pos, depth);
    std::sort(result.begin(), result.end(), compareOutputs);

    for (auto n : result)
    {
        std::cout << n.move << ": " << n.nodes << '\n';
    }
}











}