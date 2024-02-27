#pragma once
#include "types.hpp"
#include "position.hpp"

namespace Smyslov {

struct MoveGen
{
    public:
        MoveGen(Position& position) : pos(position) {}
        void generate_all_moves();
        std::vector<Move> move_list() {return moveList;};
        bool make_move(const Move input);
        bool undo_move();
        int movelist_size() {return moveList.size();};

    private:
        std::vector<Move> moveList = {};
        Smyslov::Position& pos;
        U64 checkMask = 0xffffffffffffffff;
        U64 pinnedMask = 0x0ULL;

        void generate_king_moves();
        void generate_castles();
        void generate_double_pawn_push();
        void generate_pawn_push();
        void generate_en_passant();
        void generate_pawn_captures();
        void generate_moves(Piece piece);
        void generate_captures(Piece piece);
        void generate_promotions();
        void generate_promotion_captures();
        std::optional<Move> select_move(const Move move); // choose from moveList
        bool make_enPassant(Move move);
        bool make_castle(Move move);
        bool make_capture(Move move);
        bool make_quiet(Move move);
        bool make_promotion(Move move);
        bool undo_castle(Move move);
        bool undo_capture(Move move, Piece capture);
        bool undo_enPassant();
        bool undo_quiet(Move move);
        bool undo_promotion(Move move);


};






}