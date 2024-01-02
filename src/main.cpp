#include <iostream>
#include "bitboards.hpp"
#include "position.hpp"
#include "types.hpp"
#include <string>

int main() {
    Position board;
    board.start_position();     

    while (true) {
        board.print_board();
        std::cout<<"Move: ";
        std::string my_move;
        std::cin >> my_move;
        board.make_move(my_move);
    }

}