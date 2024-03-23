#include "evaluation.hpp"

namespace Smyslov::Eval
{

int material_count(const Position& pos, Color side)
{
    int queens = population_count(pos.get_bitboard(side, Piece::QUEEN));
    int bishops = population_count(pos.get_bitboard(side, Piece::BISHOP));
    int rooks = population_count(pos.get_bitboard(side, Piece::ROOK));
    int knights = population_count(pos.get_bitboard(side, Piece::KNIGHT));
    int pawns = population_count(pos.get_bitboard(side, Piece::PAWN));

    auto queen_val = queens * PieceValue::queen;
    auto bishop_val = bishops * PieceValue::bishop;
    auto rook_val = rooks * PieceValue::rook;
    auto knight_val = knights * PieceValue::knight;
    auto pawn_val = pawns * PieceValue::pawn;

    return queen_val + bishop_val + rook_val + knight_val + pawn_val;
}

float evaluate(const Position& pos)
{
    auto friendlyMaterialCount = material_count(pos, pos.side());
    auto opMatterialCount = material_count(pos, pos.get_opposite_side());
    return friendlyMaterialCount - opMatterialCount;

}





}