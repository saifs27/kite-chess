#include "evaluation.hpp"

namespace Kite::Eval
{
int material_count(const Position& pos, Color side)
{
    int queens = population_count(pos.get_bitboard(side, Piece::QUEEN));
    int bishops = population_count(pos.get_bitboard(side, Piece::BISHOP));
    int rooks = population_count(pos.get_bitboard(side, Piece::ROOK));
    int knights = population_count(pos.get_bitboard(side, Piece::KNIGHT));
    int pawns = population_count(pos.get_bitboard(side, Piece::PAWN));

    auto queen_val = queens * PieceWeight::queen;
    auto bishop_val = bishops * PieceWeight::bishop;
    auto rook_val = rooks * PieceWeight::rook;
    auto knight_val = knights * PieceWeight::knight;
    auto pawn_val = pawns * PieceWeight::pawn;

    return queen_val + bishop_val + rook_val + knight_val + pawn_val;
}

float evaluate(const Position& pos)
{
    auto materialDiff = material_count(pos, pos.side())- material_count(pos, pos.opposite_side());
    return materialDiff;


}





}