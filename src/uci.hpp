#pragma once
#include <string>
#include "types.hpp"
#include "position.hpp"
namespace Smyslov {
namespace UCI {
std::string square_to_string(Square sq);
std::string piece_to_string(Piece piece);
std::string move_to_uci(Move move);
std::optional<Move> uci_to_move(Position& pos, std::string uci);

}
}


