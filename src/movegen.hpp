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
#include "position.hpp"
#include "types.hpp"

namespace Kite {

struct MoveGen {
   public:
    MoveGen(Position& position) : pos(position) {}
    void generate_all_moves();
    std::vector<Move> move_list() { return moveList; };
    bool make_move(const Move input);
    bool undo_move();
    int movelist_size() { return moveList.size(); };
    bool is_check();
    bool is_checkmate();
    bool is_stalemate();

   private:
    std::vector<Move> moveList = {};
    Kite::Position& pos;
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
    std::optional<Move> select_move(const Move move);  // choose from moveList

    bool make_enPassant(Move move);
    bool make_castle(Move move);
    bool make_capture(Move move);
    bool make_quiet(Move move);
    bool make_promotion(Move move);
    bool undo_castle(Move move);
    bool undo_capture(Move move, Piece capture);
    bool undo_enPassant();
    bool undo_quiet(Move move);
    bool undo_promotion(Move move, Piece captured);
};

}  // namespace Kite