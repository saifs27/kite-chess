#include "movegen.hpp"

namespace Smyslov
{


void MoveGen::generate_king_moves() 
{
    Color side = pos.side;
    Color op_side = static_cast<Color>((static_cast<int>(side) + 1) % 2);

    U64 king_pos = pos.get_bitboard(side,Piece::KING);
    Square from = lsb(king_pos);
    U64 bb = king_attacks(king_pos);
    U64 blockers = pos.pieceBB[side];
    U64 attackers = pos.get_attacks(op_side);
    U64 moves = bb & (bb ^ (blockers | attackers));
    while (moves != 0x0ULL)
    {
        Square to = pop_lsb(moves);
        Move new_move(from, to, KING, side);
        moveList.push_back(new_move);
    }
    
}

void MoveGen::generate_moves(Piece piece) 
{
    Color side = pos.side;
    Color op_side = static_cast<Color>((static_cast<int>(side) + 1) % 2);

    U64 piece_pos = pos.get_bitboard(side, piece);
    Square from;
    U64 bb;
    U64 blockers;
    U64 moves;
    U64 frombb;
    do
    {
        from = pop_lsb(piece_pos);
        frombb = set_bit(from);

        if (from == EMPTY_SQUARE) {break;}
        switch (piece)
        {
            case KNIGHT:
            bb = knight_attacks(frombb);
            break;
            case PAWN:
            bb = pawn_push(side, frombb) | double_pawn_push(side, frombb);
            bb |= (pawn_attacks(frombb) & pos.get_attacks(op_side));
            break;
            case BISHOP:
            bb = bishop_attacks(frombb);
            break;
            case ROOK:
            bb = rook_attacks(frombb);
            break;
            case QUEEN:
            bb = rook_attacks(frombb) | bishop_attacks(frombb);
            break;
            default:
            bb = 0x0ULL;
            break;
        }
        blockers = pos.pieceBB[side];
        moves = bb & (bb ^ (blockers));
        Move new_move(from, from, piece, side);

        while (moves != 0x0ULL)
        {
            Square to = pop_lsb(moves);
            new_move.to = to;
            new_move.moveType = (set_bit(to) & pos.get_attacks(op_side) != 0x0ULL) ? MoveType::CAPTURE : MoveType::NORMAL;
            moveList.push_back(new_move);
        }
    
    }
    while (from != EMPTY_SQUARE);
}

















}