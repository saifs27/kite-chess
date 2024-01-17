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
    Square from = lsb(piece_pos);

    U64 bb;
    switch (piece)
    {
        case KNIGHT:
        bb = knight_attacks(piece_pos);
        break;
        case PAWN:
        bb = pawn_push(side, piece_pos) | double_pawn_push(side, piece_pos);
        bb |= (pawn_attacks(piece_pos) & pos.get_attacks(op_side));
        break;
        case BISHOP:
        bb = bishop_attacks(piece_pos);
        break;
        case ROOK:
        bb = rook_attacks(piece_pos);
        break;
        case QUEEN:
        bb = rook_attacks(piece_pos) | bishop_attacks(piece_pos);
        break;
        default:
        bb = 0x0ULL;
        break;
    }

    U64 blockers = pos.pieceBB[side];
    U64 moves = bb & (bb ^ (blockers));
    Move new_move(from, from, piece, side);
    while (moves != 0x0ULL)
    {
        Square to = pop_lsb(moves);
        new_move.to = to;
        new_move.moveType = (set_bit(to) & pos.get_attacks(op_side) != 0x0ULL) ? MoveType::CAPTURE : MoveType::NORMAL;
        moveList.push_back(new_move);
    }
    
}

















}