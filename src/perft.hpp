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

inline int perft(Position& pos, int depth)
{
    if (pos.side() != Color::WHITE && pos.side()!= Color::BLACK)
    {
        throw std::invalid_argument(std::to_string(depth));
    }

    int initalGameHistory = pos.gameHistory.size();
    MoveGen moves(pos);
    U64 nodes = 0;
    if (depth == 0) return 1;

    moves.generate_all_moves();
    for (auto move : moves.move_list())
    {
        int initial_game_history_size = pos.gameHistory.size();
        bool isValidMove = moves.make_move(move);
        if (isValidMove)
        {
            nodes += perft(pos, depth - 1);
            bool undo = moves.undo_move();
            int current_game_history_size = pos.gameHistory.size();

            bool isValidGameHistory = (initial_game_history_size == current_game_history_size);
            if (!undo || !isValidGameHistory)
            {
              throw std::invalid_argument("can't undo.");  
            }
             
        }
    
        
    }
    bool gameHistoryIsConsistent = (pos.gameHistory.size() == initalGameHistory);
    if (!gameHistoryIsConsistent) throw std::invalid_argument("inconsistent game history");
    return nodes;
}

inline std::vector<Output> perft_verbose(Position& pos, int depth)
{
    std::vector<Output> result;
    //std::vector<Move> moveList;
    MoveGen n_moves(pos);
    U64 nodes = 0;
    if (depth == 0) return {{"null", 1ULL}};
    
    
    n_moves.generate_all_moves();

    //if (depth == 1) return {{"null", static_cast<U64>(n_moves.move_list().size())}};
    for (auto i : n_moves.move_list())
    {       

        std::string move_string = UCI::move_to_uci(i);
        U64 move_nodes = 0;


        bool isValidMove = n_moves.make_move(i);

        if (isValidMove)
        {

          
            //nodes += perft(pos, depth - 1);
            move_nodes += perft_verbose(pos, depth -1).back().nodes;

            nodes += move_nodes;
            Output output = {move_string, move_nodes};
            result.push_back(output);
            n_moves.undo_move();
        }
            
    }
    result.push_back({"Nodes searched", nodes});
    return result;
}


inline void print_perft(Position& pos, short depth)
{
    auto result = perft_verbose(pos, depth);
    std::sort(result.begin(), result.end(), compareOutputs);

    for (auto n : result)
    {
        std::cout << n.move << ": " << n.nodes << '\n';
    }
}

inline void debug_perft(Position& pos, short initial_depth)
{
    MoveGen moves(pos);
    moves.generate_all_moves();
    short depth = initial_depth;
    std::string moveString;
    while (depth >= 0)
    {
        print_perft(pos, depth);
        //int nodes = perft(pos,depth);
        //std::cout << nodes << '\n';
        --depth;
        std::cin >> moveString;
        auto move = UCI::uci_to_move(pos, moveString);
        if (move.has_value()) moves.make_move(move.value());
        else break;
    }
}











}