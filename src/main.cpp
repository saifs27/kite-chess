#include <iostream>
#include "bitboards.hpp"
#include "position.hpp"
#include "types.hpp"
#include <string>
#include "types.hpp"
#include "bitboards.hpp"
#include <vector>
#include "movegen.hpp"

int main() {
    Smyslov::Rays::init();
    //Smyslov::U64 bb = Smyslov::bishop_attacks(Smyslov::D4, 0x0ULL);
    //Smyslov::print_bitboard(bb);

    Smyslov::Position board;
    board.start_position(); 
    std::vector<Smyslov::Move> moveList;
    board.make_move("d2d4");
    board.make_move("c7c5");
    board.make_move("d4d5");
    board.make_move("e7e5");
    Smyslov::print_bitboard(board.get_bitboard(Smyslov::Color::WHITE, Smyslov::Piece::PAWN));
    Smyslov::MoveGen mList {std::move(moveList), board};

    mList.generate_all_moves();
    std::cout << mList.moveList.size() << '\n';

    for (auto i: mList.moveList)
    {
        std::cout << "move: " << static_cast<int>(i.from())<< " --> " <<static_cast<int>(i.to()) << '\n';
    }

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