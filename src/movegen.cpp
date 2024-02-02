#include "movegen.hpp"

namespace Smyslov
{


void MoveGen::generate_king_moves() 
{
    Color side = pos.side;
    Color op_side = static_cast<Color>((static_cast<int>(side) + 1) % 2);

    U64 king_pos = pos.get_bitboard(side,Piece::KING);
    Square from = lsb(king_pos);
    U64 bb = king_attacksBB(king_pos);
    U64 blockers = pos.colorsBB(side);
    U64 attackers = pos.get_attacks(op_side);
    U64 moves = bb & (bb ^ (blockers | attackers));
    while (!(is_empty(moves)))
    {
        Square to = pop_lsb(moves);
        Move new_move(from, to, Flag::QUIET);
        moveList.push_back(new_move);
    }
    
}

void MoveGen::generate_castles()
{

}

void MoveGen::generate_double_pawn_push()
{
    Color side = pos.side;
    U64 pawn_rank = (side==Color::WHITE) ? rank::second : rank::seventh;
    U64 pawn_pos = pos.get_bitboard(side, Piece::PAWN) & pawn_rank;
    
    U64 bb;
    U64 relevant_blockers = pos.colorsBB(Color::WHITE) | pos.colorsBB(Color::BLACK);
    U64 moves = double_pawn_pushBB(pawn_pos, side);
    moves &= relevant_blockers ^ moves;
    U64 frombb;
    Square from;
    Square to;
    Move new_move(Square::A1, Square::A1, Flag::DOUBLE_PAWN);

    while (!is_empty(moves))
    {
    to = pop_lsb(moves);
    frombb = (side == Color::WHITE) ? (set_bit(to) >> 16) : (set_bit(to) << 16);
    from = pop_lsb(frombb);
    new_move.set_from(from);
    new_move.set_to(to);
    moveList.push_back(new_move);

    }
    
}

void MoveGen::generate_pawn_push()
{
    Color side = pos.side;
    U64 pawn_pos = pos.get_bitboard(side, Piece::PAWN);
    U64 relevant_blockers = pos.colorsBB(Color::WHITE) | pos.colorsBB(Color::BLACK);

    Square from;
    Square to;
    U64 moves = pawn_pushBB(pawn_pos, side);
    moves &= (relevant_blockers ^ moves);
    U64 frombb;
    Move new_move(Square::A1, Square::A1, Flag::DOUBLE_PAWN);

    while (!is_empty(moves))
    {
        to = pop_lsb(moves);
        frombb = (side == Color::WHITE) ? (set_bit(to) >> 8) : (set_bit(to) << 8);     
        from = pop_lsb(frombb);  
        new_move.set_from(from);
        new_move.set_to(to);
        moveList.push_back(new_move);
    } 
    
}



void MoveGen::generate_en_passant()
{
    U64 mask = (pos.side == Color::WHITE) ? rank::sixth : rank::third;
    U64 enPas = set_bit(pos.enPassant) & mask;
    Move m(pos.enPassant, pos.enPassant, Flag::EN_PASSANT);

    if (!is_empty(enPas))
    {
        U64 pawnsBB = pos.get_bitboard(pos.side, Piece::PAWN);
        auto squares = get_squares(pawnsBB);
        for (auto square: squares)
        {
            U64 pawnAttacks = pawn_attacksBB(set_bit(square), pos.side);

            if (!is_empty(enPas & pawnAttacks))
            {
                m.set_from(square);
                moveList.push_back(m);
            }
        }         
    }
}

void MoveGen::generate_moves(Piece piece) 
{
    Color side = pos.side;
    Color op_side = (side == Color::WHITE) ? Color::WHITE : Color::BLACK;

    U64 piece_pos = pos.get_bitboard(side, piece);
    Square from;
    U64 bb;
    U64 blockers= pos.colorsBB(side);
    U64 op_blockers = pos.colorsBB(op_side);
    U64 moves;
    U64 frombb;
    do
    {
        from = pop_lsb(piece_pos);
        frombb = set_bit(from);

        if (from == Square::EMPTY_SQUARE) {break;}
        switch (piece)
        {
            case Piece::KNIGHT:
            bb = knight_attacksBB(frombb);
            break;
            case Piece::BISHOP:
            bb = bishop_attacks(from, blockers);
            break;
            case Piece::ROOK:
            bb = rook_attacks(from, blockers);
            break;
            case Piece::QUEEN:
            bb = rook_attacks(from, blockers) | bishop_attacks(from, blockers);
            break;
            default:
            bb = 0x0ULL;
            break;
        }
        
        moves = (bb & (bb ^ (blockers))) & (bb & (bb ^ (op_blockers)));
        Move new_move(from, from, Flag::QUIET);

        while (!is_empty(moves))
        {
            Square to = pop_lsb(moves);
            new_move.set_to(to);
            new_move.set_flags((set_bit(to) & pos.get_attacks(op_side) != 0x0ULL) ? Flag::CAPTURE : Flag::QUIET);
            moveList.push_back(new_move);
        }
    
    } while (from != Square::EMPTY_SQUARE);
}

void MoveGen::generate_all_moves()
{
    generate_double_pawn_push();
    generate_pawn_push();
    generate_en_passant();
    generate_moves(Piece::ROOK);
    generate_moves(Piece::BISHOP);
    generate_moves(Piece::QUEEN);
    generate_moves(Piece::KNIGHT);
    generate_king_moves();
}

bool MoveGen::make_move(Move move) 
{
    pos.enPassant = Square::A1;
    auto incrementMove50 = pos.gameState.back().fiftyMove + 1;
    auto castlingRights = pos.gameState.back().castlingPerm;
    
    GameState board(Square::A1, 0b1111, incrementMove50);

    Piece piece = pos.get_piece(move.from());

    if (!pos.is_pseudo_legal(move))
    {
        return false;
    }

    const int colorMask = pos.side == Color::WHITE ? 0b0011 : 0b1100;
    const U64 fromBB = set_bit(move.from());
    const U64 toBB = set_bit(move.to());

    if (fromBB & pos.colorsBB(pos.side) == 0 || fromBB & pos.piecesBB(piece) == 0)
    {
        return false;
    }

    if (move.flags() == Flag::DOUBLE_PAWN)
    {
        int sq = static_cast<int>(move.to());
        int backSq = (pos.side == Color::WHITE) ? (sq - 8) : (sq + 8);
        pos.enPassant = static_cast<Square>(backSq);
    }
    

    pos.moveHistory.push_back(move);
    pos.pieceBB[static_cast<int>(pos.side)] |= fromBB;
    pos.pieceBB[static_cast<int>(pos.side)] &= ~toBB;

    pos.pieceBB[static_cast<int>(piece)] |= fromBB;
    pos.pieceBB[static_cast<int>(piece)] &= ~toBB;

    pos.update_castlingPerm(move);

    pos.switch_sides();
    return true;
}

void MoveGen::undo_move() 
{
    pos.switch_sides();
    Move move = pos.moveHistory.back();
    Piece piece = pos.get_piece(move.from());
    pos.moveHistory.pop_back();

    //std::cout << static_cast<int>(move.from()) << '\n';

    const U64 prevBB = 0x1ULL << static_cast<int>(move.from());
    const U64 currentBB = 0x1ULL << static_cast<int>(move.to());
    pos.pieceBB[static_cast<int>(pos.side)] &= ~currentBB;
    pos.pieceBB[static_cast<int>(pos.side)] |= prevBB;

    pos.pieceBB[static_cast<int>(piece)] &= ~currentBB;
    pos.pieceBB[static_cast<int>(piece)] |= prevBB;

    //print_board();
}

















}