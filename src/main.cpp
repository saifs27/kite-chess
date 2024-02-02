#include <iostream>
#include "bitboards.hpp"
#include "position.hpp"
#include "types.hpp"
#include <string>
#include "types.hpp"
#include "bitboards.hpp"
#include <vector>
#include "movegen.hpp"
#include "perft.hpp"

int main() {
    Smyslov::Rays::init();
    //Smyslov::U64 bb = Smyslov::bishop_attacks(Smyslov::D4, 0x0ULL);
    //Smyslov::print_bitboard(bb);

    Smyslov::Position board;
    board.start_position(); 
    std::vector<Smyslov::Move> moveList;

    int n = Smyslov::perft(board, 4);
    std::cout << "Perft: " << n << '\n';
    /*
    board.make_move(Smyslov::Move(Smyslov::Square::D2, Smyslov::Square::D4, Smyslov::Flag::DOUBLE_PAWN));
    board.make_move(Smyslov::Move(Smyslov::Square::C7, Smyslov::Square::C5, Smyslov::Flag::DOUBLE_PAWN));
    board.make_move(Smyslov::Move(Smyslov::Square::D4, Smyslov::Square::D5, Smyslov::Flag::DOUBLE_PAWN));
    board.make_move(Smyslov::Move(Smyslov::Square::E7, Smyslov::Square::E5, Smyslov::Flag::DOUBLE_PAWN));
    Smyslov::print_bitboard(board.get_bitboard(Smyslov::Color::WHITE, Smyslov::Piece::PAWN));
    Smyslov::MoveGen mList {std::move(moveList), board};

    mList.generate_all_moves();
    std::cout << mList.moveList.size() << '\n';
    std::cout << "enPas: " << static_cast<int>(board.enPassant) << '\n';
    for (auto i: mList.moveList)
    {
        std::cout << "move: " << static_cast<int>(i.from())<< " --> " <<static_cast<int>(i.to()) << '\n';
    }


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