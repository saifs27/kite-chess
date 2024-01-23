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
    while (!(is_empty(moves)))
    {
        Square to = pop_lsb(moves);
        Move new_move(from, to, NOFLAG);
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
    U64 blockers= pos.pieceBB[side];;
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
            bb = bishop_attacks(from, blockers);
            break;
            case ROOK:
            bb = rook_attacks(from, blockers);
            break;
            case QUEEN:
            bb = rook_attacks(from, blockers) | bishop_attacks(from, blockers);
            break;
            default:
            bb = 0x0ULL;
            break;
        }
        
        moves = bb & (bb ^ (blockers));
        Move new_move(from, from, NOFLAG);

        while (!is_empty(moves))
        {
            Square to = pop_lsb(moves);
            new_move.set_to(to);
            new_move.set_flags((set_bit(to) & pos.get_attacks(op_side) != 0x0ULL) ? Flag::CAPTURE : Flag::NOFLAG);
            moveList.push_back(new_move);
        }
    
    }
    while (from != EMPTY_SQUARE);
}

void MoveGen::generate_all_moves()
{
    generate_moves(PAWN);
    generate_moves(ROOK);
    generate_moves(BISHOP);
    generate_moves(QUEEN);
    generate_moves(KNIGHT);
    generate_king_moves();
}

















}