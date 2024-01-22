#pragma once
#include "types.hpp"
#include "position.hpp"

namespace Smyslov {

struct MoveGen
{
    std::vector<Move> moveList;
    Smyslov::Position& pos;
    void generate_king_moves();
    void generate_quiet();
    void generate_captures();
    void generate_en_passant();
    void generate_moves(Piece piece);
    void generate_all_moves();
};






}