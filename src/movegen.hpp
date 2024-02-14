#pragma once
#include "types.hpp"
#include "position.hpp"

namespace Smyslov {

struct MoveGen
{
    public:
        std::vector<Move> moveList = {};
        Smyslov::Position& pos;
    private:
        void generate_king_moves();
        void generate_castles();
        void generate_double_pawn_push();
        void generate_pawn_push();
        void generate_en_passant();
        void generate_pawn_captures();
        void generate_moves(Piece piece);
        std::optional<Move> select_move(const Move move); // choose from moveList
        bool make_enPassant(Move move);
        bool make_castle(Move move);
        bool make_capture(Move move);
        bool make_quiet(Move move);
        bool undo_castle(Move move);
        bool undo_capture(Move move, Piece capture);
        bool undo_enPassant();
        bool undo_quiet(Move move);

    public:
        MoveGen(Position& position) : pos(position) {}
        void generate_all_moves();
        bool make_move(const Move input);
        bool undo_move();
};






}