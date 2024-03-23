#include <iostream>
#include <string>
#include <vector>

#include "bitboards.hpp"
#include "position.hpp"
#include "types.hpp"
#include "movegen.hpp"
#include "perft.hpp"
#include "uci.hpp"
#include "search.hpp"


void game_loop(Kite::Position& board, Kite::MoveGen moves);

int main() {
    Kite::Timer timer;
    Kite::Position board("8/2k5/3q4/8/8/8/3Q4/7K w - - 0 1");
    //board.start_position();
    //Smyslov::MoveGen moves(board);
    //moves.generate_all_moves();
    int score =  Kite::Search::search(board, 1);
    std::cout << "score: " << score << '\n';
 
    //game_loop(board, moves);
   
 



}



void game_loop(Kite::Position& board, Kite::MoveGen moves)
{
    while (true) 
    {
        if (board.score().white_score() != Kite::Result::EMPTY) {std::cout << "CHECKMATE" ; break;}
        board.print_board();
        std::string side = (board.side() == Kite::Color::WHITE) ? "White to move" : "Black to move";
        //std::cout << "fifty move: " << board.fiftyMove().value() << '\n';
        std::cout << side << '\n';

        moves.generate_all_moves();
        std::cout << "Legal moves: " << moves.movelist_size() << '\n';
        std::cout<<"Move: ";

        std::string my_move;
        std::cin >> my_move;
        if (my_move.find("undo") == std::string::npos)
        {
            auto move = Kite::UCI::uci_to_move(board, my_move);
            if (move.has_value()) moves.make_move(move.value());
        }

        else moves.undo_move();  
    }
}