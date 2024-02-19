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

    //Smyslov::print_perft(board, 4);
    std::string fen = "rnbqk2r/pppp1ppp/4pn2/8/2PP4/P1P5/4PPPP/R1BQKBNR b KQkq - 10 5";
    Smyslov::Position pos(fen);
    pos.print_board();
    while (true) 
    {
        board.print_board();
        std::string side = (board.side == Smyslov::Color::WHITE) ? "White to move" : "Black to move";
        std::cout << "fifty move: " << board.fiftyMove() << '\n';
        std::cout << "castling permissions: " << board.castlingPerms() << '\n';
        std::cout << "moveHistory size: " << board.moveHistory.size() << '\n';
        if (!board.moveHistory.empty() && board.moveHistory.back().has_capture_flag())
        {std::cout << "castle flag: kingside " << '\n';}
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