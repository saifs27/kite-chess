#pragma once
#include <array>
#include <random>

#include "types.hpp"

namespace Kite {
struct PositionKey {
   public:
    int get_piece_key(Square square, Color color, Piece piece) const;
    int get_castling_key(short castling) const;
    int get_side_key(Color perspective) const {
        return side[static_cast<int>(perspective)];
    };
    int get_en_pas_key(File en_pas) const;
    void init();
    int at(Square square, Piece piece);

   private:
    std::array<int, 768> piece_keys =
        {};  // array of each piece at each square (12 * 64)
    std::array<int, 2> side = {};
    std::array<int, 4> castling_rights = {};
    std::array<int, 8> en_pas_file = {};
};

}

}  // namespace Kite::Zobrist
