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
    Smyslov::Slider check_rook = {Smyslov::slider::rook};  
    auto n = check_rook.get_relevant_blockers(Smyslov::Square::A1);
    //auto result = Smyslov::try_offset(Smyslov::Square::D4, 1, 1).value();
    std::cout<<n.bitboard<<'\n';


    auto [a,b]= Smyslov::find_magic(check_rook, Smyslov::Square::A1, 12);
    std::cout<<"magic: "<<std::hex<<a.magic<<'\n';
    std::cout<<"mask: "<<std::hex<<a.mask<<'\n';
    std::cout<<"offset: "<<std::hex<<a.offset<<'\n';
    std::cout<<"shift: "<<a.shift<<'\n';
    std::cout << b[0].bitboard << '\n';

    /*
    while (true) {
        board.print_board();
        std::cout<<"Move: ";
        std::string my_move;
        std::cin >> my_move;
        board.make_move(my_move);
    }*/

}