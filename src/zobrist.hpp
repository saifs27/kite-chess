#pragma once
#include <array>

#include "position.hpp"

namespace Kite::Zobrist {

struct PositionKey {
   public:
    int at(Square square, Piece piece);

   private:
    std::array<int, 768> keys;
}

}  // namespace Kite::Zobrist