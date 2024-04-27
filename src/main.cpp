/*
    Kite, a UCI compliant chess engine.
    Copyright (C) 2024  Saif

    Kite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Kite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include <iostream>
#include <string>
#include <vector>

#include "bitboards.hpp"
#include "movegen.hpp"
#include "perft.hpp"
#include "position.hpp"
#include "search.hpp"
#include "types.hpp"
#include "uci.hpp"

void game_loop(Kite::Position& board, Kite::MoveGen moves);

int main() {
    Kite::Timer timer;
    Kite::Position board("8/2k5/3q4/8/8/8/3Q4/7K w - - 0 1");
    // board.start_position();
    Kite::MoveGen moves(board);
    // moves.generate_all_moves();
    auto score = Kite::Search::search(board, 1);
    std::cout << "score: " << score.score << '\n';

    auto score2 = Kite::Search::alpha_beta(board, moves, -100000, 100000, 1);
    std::cout << "alpha beta: " << score2 << '\n';

    // game_loop(board, moves);
}

void game_loop(Kite::Position& board, Kite::MoveGen moves) {
    while (true) {
        if (board.score().white_score() != Kite::Result::EMPTY) {
            std::cout << "CHECKMATE";
            break;
        }
        board.print_board();
        std::string side = (board.side() == Kite::Color::WHITE)
                               ? "White to move"
                               : "Black to move";
        // std::cout << "fifty move: " << board.fiftyMove().value() << '\n';
        std::cout << side << '\n';

        moves.generate_all_moves();
        std::cout << "Legal moves: " << moves.movelist_size() << '\n';
        std::cout << "Move: ";

        std::string my_move;
        std::cin >> my_move;
        if (my_move.find("undo") == std::string::npos) {
            auto move = Kite::UCI::uci_to_move(board, my_move);
            if (move.has_value()) moves.make_move(move.value());
        }

        else
            moves.undo_move();
    }
}