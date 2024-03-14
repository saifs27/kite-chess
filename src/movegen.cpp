#include "movegen.hpp"

namespace Smyslov
{


void MoveGen::generate_king_moves() 
{
    Color side = pos.side();
    Color op_side = static_cast<Color>((static_cast<int>(side) + 1) % 2);

    U64 king_pos = pos.get_bitboard(side, Piece::KING);
    Square from = lsb(king_pos);
    U64 bb = king_attacks(king_pos);
    U64 blockers = pos.colorsBB(side);
    U64 attackers = pos.get_attacks(op_side, blockers);
    U64 moves = bb & (bb ^ (blockers | attackers));
    Flag moveFlag = Flag::QUIET;
    //if (checkMask != 0xffffffffffffffff) moves &= ~checkMask; 
    //else moves &= checkMask;
    while (!(is_empty(moves)))
    {
        Square to = pop_lsb(moves);
        moveFlag = (pos.get_piece(to) != Piece::EMPTY) ? Flag::CAPTURE : Flag::QUIET;
        Move new_move(from, to, moveFlag);
        moveList.push_back(new_move);
    }
    
}

void MoveGen::generate_castles()
{
    U64 kingPos = pos.get_bitboard(pos.side(), Piece::KING);
    U64 rooksPos = pos.get_bitboard(pos.side(), Piece::ROOK);
    Square from = (pos.side() == Color::WHITE) ? Square::E1 : Square::E8;
    bool correctKingPos = !is_empty(kingPos & set_bit(from));
    if (!correctKingPos) return;

    Square toKingside = (pos.side() == Color::WHITE) ? Square::G1 : Square::G8;
    Square toQueenside = (pos.side() == Color::WHITE) ? Square::C1 : Square::C8;


    U64 kingsideMask = (pos.side() == Color::WHITE) ? 0x60 : 0x6000000000000000;
    U64 queensideMask = (pos.side() == Color::WHITE) ? 0xe : 0xe00000000000000;

    U64 blockers = pos.colorsBB(pos.side()) | pos.colorsBB(pos.get_opposite_side());
    U64 attackers = pos.get_attacks(pos.get_opposite_side(), 0x0ULL);
    bool canCastleKingside = is_empty((blockers | attackers) & kingsideMask);
    bool canCastleQueenside = is_empty((blockers | attackers) & queensideMask);
    Castling kingsideFlag = (pos.side() == Color::WHITE) ? Castling::WhiteKingside : Castling::BlackKingside;
    Castling queensideFlag = (pos.side() == Color::WHITE) ? Castling::WhiteQueenside : Castling::BlackQueenside; 
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
    Color side = pos.side();
    U64 pawn_rank = (side==Color::WHITE) ? rank::second : rank::seventh;
    U64 pawn_pos = pos.get_bitboard(side, Piece::PAWN) & pawn_rank;
    U64 oneSqblockerMask = (side == Color::WHITE) ? rank::third : rank::sixth;
    U64 twoSqblockerMask = (side == Color::WHITE) ? rank::fourth : rank::fifth;
    U64 bb;
    U64 relevant_blockers = (pos.colorsBB(Color::WHITE) | pos.colorsBB(Color::BLACK));

    U64 one_sq_blockers = (side == Color::WHITE) ? (relevant_blockers & oneSqblockerMask) << 8 : (relevant_blockers & oneSqblockerMask) >> 8;
    U64 two_sq_blockers = (relevant_blockers & twoSqblockerMask);
    U64 blockers = one_sq_blockers | two_sq_blockers;
    U64 moves = double_pawn_push(pawn_pos, side);
    moves &= blockers ^ moves;
    //moves &= checkMask;
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
    Color side = pos.side();
    U64 pawn_pos = pos.get_bitboard(side, Piece::PAWN);
    U64 relevant_blockers = pos.colorsBB(Color::WHITE) | pos.colorsBB(Color::BLACK);

    Square from;
    Square to;
    U64 moves = pawn_push(pawn_pos, side);
    moves &= (relevant_blockers ^ moves);
    //moves &= checkMask;
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
    U64 mask = (pos.side() == Color::WHITE) ? ~(rank::seventh | rank::eighth) : ~(rank::second | rank::first);
    U64 pawn_pos = pos.get_bitboard(pos.side(), Piece::PAWN) & mask; // to deal with promotion captures elsewhere
    U64 attacks;
    U64 opponent_pieces = pos.colorsBB(pos.get_opposite_side());
    Square from;
    Square to;
    while (!is_empty(pawn_pos))
    {
        from = pop_lsb(pawn_pos);
        attacks = pawn_attacks(set_bit(from), pos.side());
        attacks &= opponent_pieces;
        //attacks &= checkMask;
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
    U64 mask = (pos.side() == Color::WHITE) ? rank::sixth : rank::third;
    U64 enPas = set_bit(pos.enPassant()) & mask;
    //enPas &= checkMask;
    Move m(pos.enPassant(), pos.enPassant(), Flag::EN_PASSANT);
    
    if (!is_empty(enPas))
    {
        U64 pawnsBB = pos.get_bitboard(pos.side(), Piece::PAWN);
        auto squares = get_squares(pawnsBB);
        for (auto square: squares)
        {
            U64 pawnAttacks = pawn_attacks(set_bit(square), pos.side());

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
    Color side = pos.side();
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
            bb = knight_attacks(frombb);
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
        //moves &= checkMask;
        
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

void MoveGen::generate_promotions()
{
    U64 mask = (pos.side() == Color::WHITE) ? rank::seventh : rank::second;
    U64 blockerMask = (pos.side() == Color::WHITE) ? rank::eighth : rank::first;
    U64 pawns = pos.get_bitboard(pos.side(), Piece::PAWN) & mask;
    //pawns &= checkMask;
    U64 blockers = pos.colorsBB(pos.get_opposite_side()) & blockerMask;
    Move move(Square::A1, Square::A1, Flag::QUIET);
    while (!is_empty(pawns))
    {

        Square from = pop_lsb(pawns);
        U64 toBB = pawn_push_sq(from, pos.side());
        Square to = lsb(toBB);
        bool isBlocked = !is_empty(toBB & blockers);
        
        if (!isBlocked)
        {
            move.set_from(from);
            move.set_to(to);
            move.set_flags(Flag::PROMOTE_QUEEN);
            moveList.push_back(move);
            move.set_flags(Flag::PROMOTE_ROOK);
            moveList.push_back(move);
            move.set_flags(Flag::PROMOTE_KNIGHT);
            moveList.push_back(move);
            move.set_flags(Flag::PROMOTE_BISHOP);
            moveList.push_back(move);
        }
        
    }
}

void MoveGen::generate_promotion_captures()
{
    U64 mask = (pos.side() == Color::WHITE) ? rank::seventh : rank::second;
    U64 captureMask = (pos.side() == Color::WHITE) ? rank::eighth : rank::first;
    U64 pawns = pos.get_bitboard(pos.side(), Piece::PAWN) & mask;
    //pawns &= checkMask;
    U64 capturables = pos.colorsBB(pos.get_opposite_side()) & captureMask;
    Move move(Square::A1, Square::A1, Flag::QUIET);
    while (!is_empty(pawns))
    {

        Square from = pop_lsb(pawns);
        U64 toBB = pawn_attacks_sq(from, pos.side());
        toBB &= capturables;
        
        while (!is_empty(toBB))
        {
            Square to = pop_lsb(toBB);
            move.set_from(from);
            move.set_to(to);
            move.set_flags(Flag::PROMOTE_QUEEN_CAPTURE);
            moveList.push_back(move);
            move.set_flags(Flag::PROMOTE_ROOK_CAPTURE);
            moveList.push_back(move);
            move.set_flags(Flag::PROMOTE_KNIGHT_CAPTURE);
            moveList.push_back(move);
            move.set_flags(Flag::PROMOTE_BISHOP_CAPTURE);
            moveList.push_back(move);
        }
        
    }    
}

void MoveGen::generate_all_moves()
{
    if (!(moveList.empty())) moveList.clear();
    //checkMask = pos.check_mask(pos.side());
    generate_promotion_captures();
    generate_promotions();
    generate_pawn_captures();
    generate_en_passant();
    generate_castles();
    generate_double_pawn_push();
    generate_pawn_push();
    generate_moves(Piece::KNIGHT);
    generate_moves(Piece::BISHOP);
    generate_moves(Piece::ROOK);
    generate_moves(Piece::QUEEN);
    generate_king_moves();
}

std::optional<Move> MoveGen::select_move(Move move)
{
    generate_all_moves();
    if (!moveList.empty())
    {
        for (auto m : moveList)
        {
            if ((move.from() == m.from()) && (move.to() == m.to())) return m;
        }
    }

    return {};
}

bool MoveGen::is_check()
{
    return pos.is_check();
}

bool MoveGen::is_checkmate()
{

    U64 attackers = pos.pieces_attacking_king(pos.side());

    if (is_empty(attackers)) return false;
    int numberOfAttackers = population_count(attackers); // to find double checks

    U64 friendlyPieces = pos.colorsBB(pos.side());

    
    U64 blockers = (pos.colorsBB(Color::WHITE) | pos.colorsBB(Color::BLACK)) & ~attackers;
    U64 attackMask = pos.get_attacks(pos.get_opposite_side(), blockers);
    U64 kingBB = pos.get_bitboard(pos.side(), Piece::KING);
    Square kingSq = lsb(kingBB);

    generate_all_moves();
    // Only have to check king moves if it is in double check.
    if (numberOfAttackers > 1) 
    {
        for (Move move : moveList)
        {
            bool isKingMove = (move.from() == kingSq);
            bool isLegalMove = is_empty(set_bit(move.to()) & attackMask);

            if (isKingMove && isLegalMove)
            {
                return false;
            }
        }
        return true;
    }

    

    else if (numberOfAttackers == 1)
    {
        for (Move move : moveList)
        {
            make_move(move);
            bool isCheckMove = pos.is_check();
            undo_move();
            if (!isCheckMove) return false;
        }
        Result res = (pos.side() == Color::WHITE) ? BLACK_WIN : WHITE_WIN;
        pos.set_score(res);
        return true;
    }


    return false;
}


bool MoveGen::make_castle(Move move)
{
    Flag castleFlag = move.flags();
    if (castleFlag == Flag::KING_CASTLE || castleFlag == Flag::QUEEN_CASTLE)
    {
        auto incrementmove = pos.fiftyMove() + 1;

        short castlingMask = (pos.side() == Color::WHITE) ? 0b0011 : 0b1100;
        auto castling = (pos.castlingPerms()) ^ castlingMask;
        GameState state(incrementmove, castling, Piece::EMPTY, Square::A1);
        U64 fromKBB = set_bit(move.from());
        U64 toKBB = set_bit(move.to());



        pos.remove(Piece::KING, pos.side(), move.from());
        pos.add(Piece::KING, pos.side(), move.to());
        

        Square RookSq;
        Square RookSqTo;

        switch (castleFlag)
        {
            case Flag::KING_CASTLE:
                RookSq = (pos.side() == Color::WHITE) ? Square::H1 : Square::H8;
                RookSqTo = (pos.side() == Color::WHITE) ? Square::F1 : Square::F8;
                break;
            case Flag::QUEEN_CASTLE:
                RookSq = (pos.side() == Color::WHITE) ? Square::A1 : Square::A8;
                RookSqTo = (pos.side() == Color::WHITE) ? Square::D1 : Square::D8;
                break;

        }

        U64 fromRBB = set_bit(RookSq);
        U64 toRBB = set_bit(RookSqTo);

        pos.remove(Piece::ROOK, pos.side(), RookSq);
        pos.add(Piece::ROOK, pos.side(), RookSqTo);
        

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

        pos.remove(piece, pos.side(), move.from());
        pos.remove(captured, pos.get_opposite_side(), move.to());
        pos.add(piece, pos.side(), move.to());



        return true;
    }
    return false;
}

bool MoveGen::make_quiet(const Move move)
{
    Piece piece = pos.get_piece(move.from());
    const U64 fromBB = set_bit(move.from());
    const U64 toBB = set_bit(move.to());
    if (fromBB & pos.colorsBB(pos.side()) == 0 || fromBB & pos.piecesBB(piece) == 0)
    {
        return false;
    }

    pos.remove(piece, pos.side(), move.from());
    pos.add(piece, pos.side(), move.to());
    
    if (move.flags() == Flag::DOUBLE_PAWN)
    {
        int sq = static_cast<int>(move.to());
        int backSq = (pos.side() == Color::WHITE) ? (sq - 8) : (sq + 8);

    }

    return true;
}

bool MoveGen::make_enPassant(Move move)
{
    Square enPassantSq = (pos.enPassant());
    auto capturedPawn = (pos.side() == Color::WHITE) ? try_offset(enPassantSq, 0, -1) : try_offset(enPassantSq, 0, 1);

    if ((move.flags() == Flag::EN_PASSANT) && (enPassantSq != Square::A1) && (capturedPawn.has_value()))
    {
        
        pos.remove(Piece::PAWN, pos.get_opposite_side(), capturedPawn.value());
        pos.remove(Piece::PAWN, pos.side(), move.from());
        pos.add(Piece::PAWN, pos.side(), enPassantSq);
        return true;
    }
    return false;
}


bool MoveGen::make_promotion(Move move)
{
    if (move.has_promotion_flag())
    {
        Piece promotedPiece;
        Piece captured = pos.get_piece(move.to());
        switch (move.flags())
        {
            case Flag::PROMOTE_QUEEN: case Flag::PROMOTE_QUEEN_CAPTURE:
                promotedPiece = Piece::QUEEN;
                break;
            case Flag::PROMOTE_ROOK: case Flag::PROMOTE_ROOK_CAPTURE:
                promotedPiece = Piece::ROOK;
                break;
            case Flag::PROMOTE_KNIGHT: case Flag::PROMOTE_KNIGHT_CAPTURE:
                promotedPiece = Piece::KNIGHT;
                break;
            case Flag::PROMOTE_BISHOP: case Flag::PROMOTE_BISHOP_CAPTURE:
                promotedPiece = Piece::BISHOP;
                break;
            default:
                return false;
        }

        if (captured != Piece::EMPTY) 
        {
            pos.remove(captured, pos.get_opposite_side(), move.to());
        }
        pos.remove(Piece::PAWN, pos.side(), move.from());
        pos.add(promotedPiece, pos.side(), move.to());
        return true;
    }

    return false;

}

bool MoveGen::make_move(const Move input) 
{
    
    auto n_move = select_move(input);
    if (!n_move.has_value()) {return false;}
    Move move = n_move.value();

    if (pos.score().white_score() != Result::EMPTY) return false;

    GameState game_state = pos.new_gameState(move);
    //pos.gameState.push_back(game_state); 
    Piece piece = pos.get_piece(move.from());
    bool isValid = false;
    switch(move.flags())
    {
        case Flag::QUIET: case Flag::DOUBLE_PAWN:
            isValid = make_quiet(move);    
            if (pos.is_check()) {undo_quiet(move); isValid = false; pos.gameState.pop_back();}
            break;
        case Flag::KING_CASTLE: case Flag::QUEEN_CASTLE:
            isValid = make_castle(move);
            if (pos.is_check()) {undo_castle(move); isValid = false; pos.gameState.pop_back();}
            break;
        case Flag::CAPTURE:
            isValid =  make_capture(move);
            if (pos.is_check()) {undo_capture(move, game_state.captured); isValid = false; pos.gameState.pop_back();} 
            break;
        case Flag::EN_PASSANT:
            isValid = make_enPassant(move); 
            if (pos.is_check()) {undo_capture(move, Piece::PAWN); isValid = false; pos.gameState.pop_back();}
            break;
        default:
            isValid = make_promotion(move);
            if (pos.is_check()) {undo_promotion(move, game_state.captured); isValid = false; pos.gameState.pop_back();}
            break;
    }


    
    if (isValid)
    {
        pos.moveHistory.push_back(move);
        pos.switch_sides();
        pos.gameState.push_back(game_state); 
    }


    return isValid;

}
bool MoveGen::undo_quiet(Move move)
{
        Piece piece = pos.get_piece(move.to());
        const U64 prevBB = set_bit(move.from());
        const U64 currentBB = set_bit(move.to());

        pos.remove(piece, pos.side(), move.to());
        pos.add(piece, pos.side(), move.from());
        
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
                Square originalRookSq = (pos.side() == Color::WHITE) ? Square::H1 : Square::H8;
                Square castledRookSq = (pos.side() == Color::WHITE) ? Square::F1 : Square::F8;

                pos.remove(Piece::ROOK, pos.side(), castledRookSq);
                pos.add(Piece::ROOK, pos.side(), originalRookSq);

                return true;
            }

            case Flag::QUEEN_CASTLE:
            {
                Square originalRookSq = (pos.side() == Color::WHITE) ? Square::A1 : Square::A8;
                Square castledRookSq = (pos.side() == Color::WHITE) ? Square::D1 : Square::D8;

                pos.remove(Piece::ROOK, pos.side(), castledRookSq);
                pos.add(Piece::ROOK, pos.side(), originalRookSq);

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
                const Square enPasSq = pos.enPassant();
                const U64 enPasBB = set_bit(enPasSq);
                const Square capturedEnPas = (pos.side() == Color::WHITE) ? try_offset(enPasSq, 0, -1).value() : try_offset(enPasSq, 0, 1).value();
                pos.add(Piece::PAWN, pos.side(), capturedEnPas);
                pos.add(Piece::PAWN, pos.get_opposite_side(), capturedEnPas);
                return true;
            }
        pos.add(capture, pos.get_opposite_side(), move.to());
        return true;
    }
    return false;  
    
}

bool MoveGen::undo_promotion(Move move, Piece captured)
{

    Piece piece = pos.get_piece(move.to());
    const U64 prevBB = set_bit(move.from());
    const U64 currentBB = set_bit(move.to());

    pos.remove(piece, pos.side(), move.to());
    pos.add(Piece::PAWN, pos.side(), move.from());

    if (captured != Piece::EMPTY)
    {
        pos.add(captured, pos.get_opposite_side(), move.to());
    }
    
    return true;
}

bool MoveGen::undo_move() 
{
    if (!pos.moveHistory.empty())
    {

        if (pos.score().white_score() != Result::EMPTY)
        {
            pos.set_score(Result::EMPTY);
        }

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
                return undo_promotion(move, g.captured);
        }        
    }
    generate_all_moves();
    return false;
}

















}