#include <iostream>
#include "bitboards.hpp"
#include "position.hpp"
#include "types.hpp"
#include <string>
#include "magic.hpp"
#include "types.hpp"

int main() {
    Smyslov::Position board;
    board.start_position(); 
    std::cout<<sizeof(Smyslov::Bitboard)<<'\n';
    std::cout<<sizeof(Smyslov::U64)<<'\n';
    std::cout<<sizeof(Smyslov::Slider);
    while (true) 
    {
        board.print_board();
        std::cout<<"Move: ";
        std::string my_move;
        std::cin >> my_move;
        board.make_move(my_move);
    }

}