#pragma once
#include "types.hpp"
#include "position.hpp"

namespace Smyslov {

struct MoveGen
{
    std::vector<Move> moveList = {};
    Smyslov::Position& pos;
    void generate_king_moves();
    void generate_castles();
    void generate_double_pawn_push();
    void generate_pawn_push();
    void generate_en_passant();
    void generate_moves(Piece piece);
    void generate_all_moves();
    
    bool make_move(Move move);
    void undo_move();
};






}