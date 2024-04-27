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

#pragma once
#include <string>

#include "position.hpp"
#include "types.hpp"
namespace Kite {
namespace UCI {
inline const std::string engine_version = "Kite 2024";
std::string square_to_string(Square sq);
std::string piece_to_string(Piece piece);
Piece string_to_piece(const char string);
std::string move_to_uci(Move move);
std::optional<Move> uci_to_move(Position& pos, std::string uci);
std::vector<std::string> split(std::string str, char split);
void uci_loop();

}  // namespace UCI
}  // namespace Kite
