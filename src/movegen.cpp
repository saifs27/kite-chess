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
    U64 kingPos = pos.get_bitboard(pos.side, Piece::KING);
    U64 rooksPos = pos.get_bitboard(pos.side, Piece::ROOK);
    Square from = (pos.side == Color::WHITE) ? Square::E1 : Square::E8;
    bool correctKingPos = !is_empty(kingPos & set_bit(from));
    if (!correctKingPos) return;

    Square toKingside = (pos.side == Color::WHITE) ? Square::G1 : Square::G8;
    Square toQueenside = (pos.side == Color::WHITE) ? Square::C1 : Square::C8;


    U64 kingsideMask = (pos.side == Color::WHITE) ? 0x60 : 0x6000000000000000;
    U64 queensideMask = (pos.side == Color::WHITE) ? 0xe : 0xe00000000000000;

    U64 blockers = pos.colorsBB(pos.side) | pos.colorsBB(pos.get_opposite_side());
    U64 attackers = pos.get_attacks(pos.get_opposite_side());
    bool canCastleKingside = is_empty((blockers | attackers) & kingsideMask);
    bool canCastleQueenside = is_empty((blockers | attackers) & queensideMask);
    Castling kingsideFlag = (pos.side == Color::WHITE) ? Castling::WhiteKingside : Castling::BlackKingside;
    Castling queensideFlag = (pos.side == Color::WHITE) ? Castling::WhiteQueenside : Castling::BlackQueenside; 
    if (canCastleKingside && pos.can_castle(kingsideFlag))
    {
        Move castleK(from, toKingside, Flag::KING_CASTLE);
        moveList.push_back(castleK);
    }

    if (canCastleQueenside && pos.can_castle(queensideFlag))
    {
        Move castleQ(from, toQueenside, Flag::QUEEN_CASTLE);
        moveList.push_back(castleQ);
    }


}

void MoveGen::generate_double_pawn_push()
{
    Color side = pos.side;
    U64 pawn_rank = (side==Color::WHITE) ? rank::second : rank::seventh;
    U64 pawn_pos = pos.get_bitboard(side, Piece::PAWN) & pawn_rank;
    U64 oneSqblockerMask = (side == Color::WHITE) ? rank::third : rank::sixth;
    U64 twoSqblockerMask = (side == Color::WHITE) ? rank::fourth : rank::fifth;
    U64 bb;
    U64 relevant_blockers = (pos.colorsBB(Color::WHITE) | pos.colorsBB(Color::BLACK));

    U64 one_sq_blockers = (side == Color::WHITE) ? (relevant_blockers & oneSqblockerMask) << 8 : (relevant_blockers & oneSqblockerMask) >> 8;
    U64 two_sq_blockers = (relevant_blockers & twoSqblockerMask);
    U64 blockers = one_sq_blockers | two_sq_blockers;
    U64 moves = double_pawn_pushBB(pawn_pos, side);
    moves &= blockers ^ moves;
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
    Move new_move(Square::A1, Square::A1, Flag::QUIET);

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

void MoveGen::generate_pawn_captures()
{
    U64 mask = (pos.side == Color::WHITE) ? ~(rank::seventh | rank::eighth) : ~(rank::second | rank::first);
    U64 pawn_pos = pos.get_bitboard(pos.side, Piece::PAWN) & mask; // to deal with promotion captures elsewhere
    U64 attacks;
    U64 opponent_pieces = pos.colorsBB(pos.get_opposite_side());
    Square from;
    Square to;
    while (!is_empty(pawn_pos))
    {
        from = pop_lsb(pawn_pos);
        attacks = pawn_attacksBB(set_bit(from), pos.side);
        attacks &= opponent_pieces;
        while (!is_empty(attacks))
        {
            to = pop_lsb(attacks);
            Move move(from, to, Flag::CAPTURE);
            moveList.push_back(move);
        }
    }

}

void MoveGen::generate_en_passant()
{
    U64 mask = (pos.side == Color::WHITE) ? rank::sixth : rank::third;
    U64 enPas = set_bit(pos.enPassant()) & mask;
    Move m(pos.enPassant(), pos.enPassant(), Flag::EN_PASSANT);
    
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
    Color op_side = pos.get_opposite_side();

    U64 piece_pos = pos.get_bitboard(side, piece);
    Square from;
    Square to;
    U64 bb;
    U64 blockers= pos.colorsBB(side) | pos.colorsBB(op_side);
    U64 op_blockers = pos.colorsBB(op_side);
    U64 moves;
    U64 frombb;
    U64 capture_moves;
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
        moves = bb & (bb ^ blockers);
        
        capture_moves = bb & op_blockers;
        Move new_move(from, from, Flag::QUIET);

        while (!is_empty(moves))
        {
            to = pop_lsb(moves);
            new_move.set_to(to);
            moveList.push_back(new_move);
        }

        while (!is_empty(capture_moves))
        {
            to = pop_lsb(capture_moves);
            new_move.set_to(to);
            new_move.set_flags(Flag::CAPTURE);
            moveList.push_back(new_move);
        }
    
    } while (from != Square::EMPTY_SQUARE);
}

void MoveGen::generate_all_moves()
{
    if (!(moveList.empty())) moveList.clear();
    generate_pawn_captures();

    generate_double_pawn_push();
    generate_castles();
    generate_pawn_push();
    generate_en_passant();
    generate_moves(Piece::ROOK);
    generate_moves(Piece::BISHOP);
    generate_moves(Piece::QUEEN);
    generate_moves(Piece::KNIGHT);
    generate_king_moves();
}

std::optional<Move> MoveGen::select_move(Move move)
{
    if (!moveList.empty())
    {
        for (auto m : moveList)
        {
            if ((move.from() == m.from()) && (move.to() == m.to()))
            {
                return m;
            }
        }
    }

    return {};
}


bool MoveGen::make_castle(Move move)
{
    Flag castleFlag = move.flags();
    if (castleFlag == Flag::KING_CASTLE || castleFlag == Flag::QUEEN_CASTLE)
    {
        auto incrementmove = pos.fiftyMove() + 1;

        short castlingMask = (pos.side == Color::WHITE) ? 0b0011 : 0b1100;
        auto castling = (pos.castlingPerms()) ^ castlingMask;
        GameState state(incrementmove, castling, Piece::EMPTY, Square::A1);
        U64 fromKBB = set_bit(move.from());
        U64 toKBB = set_bit(move.to());



        pos.remove(Piece::KING, pos.side, move.from());
        pos.add(Piece::KING, pos.side, move.to());
        

        Square RookSq;
        Square RookSqTo;

        switch (castleFlag)
        {
            case Flag::KING_CASTLE:
                RookSq = (pos.side == Color::WHITE) ? Square::H1 : Square::H8;
                RookSqTo = (pos.side == Color::WHITE) ? Square::F1 : Square::F8;
                break;
            case Flag::QUEEN_CASTLE:
                RookSq = (pos.side == Color::WHITE) ? Square::A1 : Square::A8;
                RookSqTo = (pos.side == Color::WHITE) ? Square::D1 : Square::D8;
                break;

        }

        U64 fromRBB = set_bit(RookSq);
        U64 toRBB = set_bit(RookSqTo);

        pos.remove(Piece::ROOK, pos.side, RookSq);
        pos.add(Piece::ROOK, pos.side, RookSqTo);
        

        pos.push_move(move);

        return true;

    }
    return false;

}

bool MoveGen::make_capture(const Move move)
{
    
    if (move.has_capture_flag())
    {
        const U64 fromBB = set_bit(move.from());
        const U64 toBB = set_bit(move.to());
        Piece captured = pos.get_piece(move.to());
        Piece piece = pos.get_piece(move.from());

        pos.remove(piece, pos.side, move.from());
        pos.remove(captured, pos.get_opposite_side(), move.to());
        pos.add(piece, pos.side, move.to());

        GameState board(0, 15, captured, Square::A1);

        return true;
    }
    return false;
}

bool MoveGen::make_quiet(const Move move)
{
    Piece piece = pos.get_piece(move.from());
    const U64 fromBB = set_bit(move.from());
    const U64 toBB = set_bit(move.to());
    if (fromBB & pos.colorsBB(pos.side) == 0 || fromBB & pos.piecesBB(piece) == 0)
    {
        return false;
    }

    pos.remove(piece, pos.side, move.from());
    pos.add(piece, pos.side, move.to());
    
    if (move.flags() == Flag::DOUBLE_PAWN)
    {
        int sq = static_cast<int>(move.to());
        int backSq = (pos.side == Color::WHITE) ? (sq - 8) : (sq + 8);

    }

    return true;
}

bool MoveGen::make_enPassant(Move move)
{
    Square enPassantSq = (pos.enPassant());
    auto capturedPawn = (pos.side == Color::WHITE) ? try_offset(enPassantSq, 0, -1) : try_offset(enPassantSq, 0, 1);

    if ((move.flags() == Flag::EN_PASSANT) && (enPassantSq != Square::A1) && (capturedPawn.has_value()))
    {
        
        pos.remove(Piece::PAWN, pos.get_opposite_side(), capturedPawn.value());
        pos.remove(Piece::PAWN, pos.side, move.from());
        pos.add(Piece::PAWN, pos.side, enPassantSq);
        return true;
    }
    return false;
}

bool MoveGen::make_move(const Move input) 
{
    
    auto n_move = select_move(input);
    if (!n_move.has_value()) {return false;}
    Move move = n_move.value();
    pos.update_gameState(move);
    Piece piece = pos.get_piece(move.from());
    bool isValid = false;
    switch(move.flags())
    {
        case Flag::QUIET: case Flag::DOUBLE_PAWN:
        isValid = make_quiet(move); break;
        case Flag::KING_CASTLE: case Flag::QUEEN_CASTLE:
        isValid = make_castle(move); break;
        case Flag::CAPTURE:
        isValid =  make_capture(move); break;
        case Flag::EN_PASSANT:
        isValid = make_enPassant(move); break;
        default:
        isValid = false; break;
    }
    
    if (isValid)
    {
        pos.moveHistory.push_back(move);
        pos.switch_sides();        
    }
    else { pos.gameState.pop_back();}

    return isValid;

}
bool MoveGen::undo_quiet(Move move)
{

        Piece piece = pos.get_piece(move.to());

        const U64 prevBB = set_bit(move.from());
        const U64 currentBB = set_bit(move.to());

        pos.remove(piece, pos.side, move.to());
        pos.add(piece, pos.side, move.from());
        
        return true;   
}
bool MoveGen::undo_castle(Move move)
{
    if (move.flags() == Flag::KING_CASTLE || move.flags() == Flag::QUEEN_CASTLE)
    {
        undo_quiet(move);

        switch (move.flags())
        {
            case Flag::KING_CASTLE:
            {
                Square originalRookSq = (pos.side == Color::WHITE) ? Square::H1 : Square::H8;
                Square castledRookSq = (pos.side == Color::WHITE) ? Square::F1 : Square::F8;

                pos.remove(Piece::ROOK, pos.side, castledRookSq);
                pos.add(Piece::ROOK, pos.side, originalRookSq);

                return true;
            }

            case Flag::QUEEN_CASTLE:
            {
                Square originalRookSq = (pos.side == Color::WHITE) ? Square::A1 : Square::A8;
                Square castledRookSq = (pos.side == Color::WHITE) ? Square::D1 : Square::D8;

                pos.remove(Piece::ROOK, pos.side, castledRookSq);
                pos.add(Piece::ROOK, pos.side, originalRookSq);

                return true;        
            }
        }
    
    }
    
    return false;
}



bool MoveGen::undo_capture(Move move, Piece capture)
{
    if (move.has_capture_flag() && capture != Piece::EMPTY)
    {
        undo_quiet(move);
        const U64 prevBB = set_bit(move.from());
        const U64 currentBB = set_bit(move.to());
        if (move.flags() == Flag::EN_PASSANT && capture !=Piece::EMPTY)
            {
                Square enPasSq = pos.enPassant();
                U64 enPasBB = set_bit(enPasSq);
                Square capturedEnPas = (pos.side == Color::WHITE) ? try_offset(enPasSq, 0, -1).value() : try_offset(enPasSq, 0, 1).value();
                pos.add(Piece::PAWN, pos.side, capturedEnPas);
                pos.add(Piece::PAWN, pos.get_opposite_side(), capturedEnPas);
                return true;
            }
        pos.add(capture, pos.get_opposite_side(), move.to());
        return true;
    }
    return false;  
    
}
bool MoveGen::undo_move() 
{
    if (!pos.moveHistory.empty())
    {
        pos.switch_sides();
        Move move = pos.moveHistory.back();
        pos.moveHistory.pop_back();
        GameState& g = pos.gameState.back();
        pos.gameState.pop_back();
        

        switch (move.flags())
        {
            case Flag::QUIET: case Flag::DOUBLE_PAWN:
                return undo_quiet(move);
            case Flag::KING_CASTLE: case Flag::QUEEN_CASTLE:
                return undo_castle(move);
            case Flag::EN_PASSANT: case Flag::CAPTURE:
            {
                return undo_capture(move, g.captured);
            }
            default:
                return false;
        }        
    }
    return false;
}

















}