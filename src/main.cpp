#include <iostream>
#include <string>
#include <vector>

#include "bitboards.hpp"
#include "position.hpp"
#include "types.hpp"
#include "movegen.hpp"
#include "perft.hpp"
#include "uci.hpp"

int main() {
//"r1bqkbnr/pP3ppp/2n5/4p3/8/8/PPPP1PPP/RNBQKBNR w KQkq - 1 5"
    Smyslov::Position board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    board.start_position();
    Smyslov::MoveGen moves(board);
    moves.generate_all_moves();

    Smyslov::Position perft_ex("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    //Smyslov::print_perft(board, 4);
    Smyslov::print_perft(perft_ex, 3);
    /*
    
    std::string fen = "rnbqkbnr/ppp1pppp/8/1B1p4/4P3/8/PPPP1PPP/RNBQK1NR b KQkq - 1 2";
    Smyslov::Position pos(fen);
    Smyslov::U64 blockers = pos.colorsBB(Smyslov::Color::WHITE) | pos.colorsBB(Smyslov::Color::BLACK);
    Smyslov::U64 checksbb = pos.check_mask(Smyslov::Color::BLACK);
    //Smyslov::U64 checksbb = rook_attacks(Smyslov::Square::A1, 2);
    Smyslov::print_bitboard(checksbb);
    pos.print_board();
    */
    
    while (true) 
    {
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
            if (move.has_value())
            {
                
                moves.make_move(move.value());
            }
        }

        else
        {
            moves.undo_move();
        }
        
      

    }
     



}