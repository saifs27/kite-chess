#include <iostream>
#include "bitboards.hpp"
#include "position.hpp"
#include "types.hpp"
#include <string>
#include "magic.hpp"
#include "types.hpp"
#include "bitboards.hpp"
#include <vector>
#include "movegen.hpp"

int main() {
    Smyslov::Rays::init();
    Smyslov::U64 bb = Smyslov::bishop_attacks(Smyslov::D4, 0x0ULL);
    Smyslov::print_bitboard(bb);

    Smyslov::Position board;
    board.start_position(); 
    std::vector<Smyslov::Move> moveList;
    board.make_move("e2e4");
    board.make_move("e7e5");
    Smyslov::MoveGen mList {std::move(moveList), board};

    mList.generate_moves(Smyslov::BISHOP);

    for (auto i: mList.moveList)
    {
        std::cout << i.to << '\n';
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