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

#include "evaluation.hpp"

namespace Kite::Eval {
int material_count(const Position& pos, Color side) {
    int queens =
        Bitboard::population_count(pos.get_bitboard(side, Piece::QUEEN));
    int bishops =
        Bitboard::population_count(pos.get_bitboard(side, Piece::BISHOP));
    int rooks = Bitboard::population_count(pos.get_bitboard(side, Piece::ROOK));
    int knights =
        Bitboard::population_count(pos.get_bitboard(side, Piece::KNIGHT));
    int pawns = Bitboard::population_count(pos.get_bitboard(side, Piece::PAWN));

    auto queen_val = queens * PieceWeight::queen;
    auto bishop_val = bishops * PieceWeight::bishop;
    auto rook_val = rooks * PieceWeight::rook;
    auto knight_val = knights * PieceWeight::knight;
    auto pawn_val = pawns * PieceWeight::pawn;

    return queen_val + bishop_val + rook_val + knight_val + pawn_val;
}

float evaluate(const Position& pos) {
    auto materialDiff = material_count(pos, pos.side()) -
                        material_count(pos, pos.opposite_side());
    return materialDiff;
}

}  // namespace Kite::Eval