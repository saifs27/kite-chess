#include <string>
#include "types.hpp"
namespace Smyslov {
namespace UCI {
std::string square_to_string(Square sq);
std::string piece_to_string(Piece piece);
std::string move_to_uci(Move move);
Move uci_to_move(std::string);









}
}


