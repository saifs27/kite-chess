#pragma once
#include "types.hpp"
#include "position.hpp"

namespace Smyslov {

struct MoveGen
{
    std::vector<Move> moveList = {};
    Smyslov::Position& pos;

    MoveGen(Position& position) : pos(position) {}
    void generate_king_moves();
    void generate_castles();
    void generate_double_pawn_push();
    void generate_pawn_push();
    void generate_en_passant();
    void generate_pawn_captures();
    void generate_moves(Piece piece);
    void generate_all_moves();
    std::optional<Move> select_move(const Move move); // choose from moveList
    bool make_enPassant(Move move);
    bool make_castle(Move move);
    bool make_move(const Move input);
    void undo_move();
};






}