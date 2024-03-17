#include <iostream>
#include <string>
#include <vector>

#include "bitboards.hpp"
#include "position.hpp"
#include "types.hpp"
#include "movegen.hpp"
#include "perft.hpp"
#include "uci.hpp"


void game_loop(Smyslov::Position& board, Smyslov::MoveGen moves);

int main() {
//"r1bqkbnr/pP3ppp/2n5/4p3/8/8/PPPP1PPP/RNBQKBNR w KQkq - 1 5"
    //Smyslov::Position board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    Smyslov::Position board("rnbqkbnr/ppp1pppp/8/3p4/6P1/7B/PPPPPP1P/RNBQK1NR b KQkq - 1 2");
    board.start_position();
    Smyslov::MoveGen moves(board);
    moves.generate_all_moves();
    Smyslov::Position perft_ex("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

    Smyslov::print_perft(board, 5);

 
    //game_loop(board, moves);
   
 



}



void game_loop(Smyslov::Position& board, Smyslov::MoveGen moves)
{
    while (true) 
    {
        if (board.score().white_score() != Smyslov::Result::EMPTY) {std::cout << "CHECKMATE" ; break;}
        board.print_board();
        std::string side = (board.side() == Smyslov::Color::WHITE) ? "White to move" : "Black to move";
        std::cout << "fifty move: " << board.fiftyMove() << '\n';
        std::cout << side << '\n';

        moves.generate_all_moves();
        std::cout << "Legal moves: " << moves.movelist_size() << '\n';
        std::cout<<"Move: ";

        std::string my_move;
        std::cin >> my_move;
        if (my_move.find("undo") == std::string::npos)
        {
            auto move = Smyslov::UCI::uci_to_move(board, my_move);
            if (move.has_value()) moves.make_move(move.value());
        }

        else moves.undo_move();  
    }
}