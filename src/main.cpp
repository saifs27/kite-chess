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
    //Smyslov::U64 bb = Smyslov::bishop_attacks(Smyslov::D4, 0x0ULL);
    //Smyslov::print_bitboard(bb);

    Smyslov::Position board;
    board.start_position(); 
    Smyslov::MoveGen moves(board);
    moves.generate_all_moves();
    board.print_board();

    for (auto i : moves.moveList) {
        std::string res = Smyslov::UCI::move_to_uci(i);
        std::cout<< res << '\n';
    }

    //int n = Smyslov::perft(board, 3);
    //std::cout << "Perft: " << n << '\n';
    /*

    while (true) 
    {
        board.print_board();
        std::cout<<"Move: ";
        std::string my_move;
        std::cin >> my_move;
        board.make_move(my_move);
    }

    */

}