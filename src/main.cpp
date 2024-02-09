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
    Smyslov::Rays::init();

    Smyslov::Position board;
    board.start_position(); 
    Smyslov::MoveGen moves(board);
    moves.generate_all_moves();

    //int n = Smyslov::perft(board, 3);
    //std::cout << "Perft: " << n << '\n';

    

    while (true) 
    {
        board.print_board();
        std::string side = (board.side == Smyslov::Color::WHITE) ? "White to move" : "Black to move";
        std::cout << "fifty move: " << board.get_fiftyMove() << '\n';
        std::cout << side << '\n';
        std::cout<<"Move: ";
        
        std::string my_move;
        std::cin >> my_move;
        if (my_move.find("undo") == std::string::npos)
        {
            auto move = Smyslov::UCI::uci_to_move(board, my_move);
            if (move.has_value())
            {
                moves.generate_all_moves();
                moves.make_move(move.value());
            }
        }

        else
        {
            moves.undo_move();
        }
        

    }



}