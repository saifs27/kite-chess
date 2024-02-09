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

    Square toKingside = (pos.side == Color::WHITE) ? Square::G1 : Square::G8;
    Square toQueenside = (pos.side == Color::WHITE) ? Square::C1 : Square::C8;


    U64 kingsideMask = (pos.side == Color::WHITE) ? 0x60 : 0x6000000000000000;
    U64 queensideMask = (pos.side == Color::WHITE) ? 0xe : 0xe00000000000000;

    U64 blockers = pos.colorsBB(pos.side) & pos.colorsBB(pos.get_opposite_side());
    U64 attackers = pos.get_attacks(pos.get_opposite_side());
    bool canCastleKingside = is_empty((blockers | attackers) & kingsideMask);
    bool canCastleQueenside = is_empty((blockers | attackers) & queensideMask);
    if (canCastleKingside)
    {
        Move castleK(from, toKingside, Flag::KING_CASTLE);
        moveList.push_back(castleK);
    }

    if (canCastleQueenside)
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
    U64 enPas = set_bit(pos.get_enPassant()) & mask;
    Move m(pos.get_enPassant(), pos.get_enPassant(), Flag::EN_PASSANT);
    
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
    if (!(moveList.empty())) moveList.clear();
    
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


void MoveGen::make_castle(Move move)
{
    Flag castleFlag = move.flags();
    if (castleFlag == Flag::KING_CASTLE || castleFlag == Flag::QUEEN_CASTLE)
    {
        auto incrementmove = pos.get_fiftyMove() + 1;

        short castlingMask = (pos.side == Color::WHITE) ? 0b0011 : 0b1100;
        auto castling = (pos.get_castlingPerms()) ^ castlingMask;
        GameState state(Square::A1, castling, incrementmove);
        U64 fromKBB = set_bit(move.from());
        U64 toKBB = set_bit(move.to());

        pos.pieceBB[static_cast<int>(Piece::KING)] &= ~fromKBB;
        pos.pieceBB[static_cast<int>(Piece::KING)] |= toKBB;
        pos.pieceBB[static_cast<int>(pos.side)] &= ~fromKBB;
        pos.pieceBB[static_cast<int>(pos.side)] |= toKBB;

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

        pos.pieceBB[static_cast<int>(Piece::ROOK)] &= ~fromRBB;
        pos.pieceBB[static_cast<int>(Piece::ROOK)] |= toRBB;
        pos.pieceBB[static_cast<int>(pos.side)] &= ~fromRBB;
        pos.pieceBB[static_cast<int>(pos.side)] |= toRBB;

        pos.gameState.push_back(state);    

    }


}

bool MoveGen::make_move(const Move input) 
{
    auto n_move = select_move(input);
    if (!n_move.has_value()) {return false;}
    Move move = n_move.value();

    auto incrementMove50 = pos.get_fiftyMove() + 1;
    auto castlingRights = pos.get_castlingPerms();
    
    GameState board(Square::A1, castlingRights, incrementMove50);
    make_castle(move);
    Piece piece = pos.get_piece(move.from());


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
        pos.gameState.back().enPassant = static_cast<Square>(backSq);
    }
    
    


    //if (pos.check_square_color(move.to()) == pos.get_opposite_side())
    if (!is_empty(pos.pieceBB[static_cast<int>(pos.get_opposite_side())] & set_bit(move.to())))
    {
        pos.pieceBB[static_cast<int>(pos.get_opposite_side())] &= ~toBB;
        move.switch_flag_to_capture();

        pos.captured.push_back(pos.get_piece(move.to()));
    }

    pos.pieceBB[static_cast<int>(pos.side)] |= toBB;
    pos.pieceBB[static_cast<int>(pos.side)] &= ~fromBB;

    pos.pieceBB[static_cast<int>(piece)] |= toBB;
    pos.pieceBB[static_cast<int>(piece)] &= ~fromBB;

    pos.push_move(move);
    auto newCastlingPerm = pos.update_castlingPerm(move);
    GameState new_gameState(Square::A1, newCastlingPerm, 50);
    
    pos.switch_sides();
    return true;
}

void MoveGen::undo_move() 
{
    pos.switch_sides();
    Move move = pos.moveHistory.back();
    Piece piece = pos.get_piece(move.to());
    pos.moveHistory.pop_back();

    //std::cout << static_cast<int>(move.from()) << '\n';

    const U64 prevBB = 0x1ULL << static_cast<int>(move.from());
    const U64 currentBB = 0x1ULL << static_cast<int>(move.to());
    pos.pieceBB[static_cast<int>(pos.side)] &= ~currentBB;
    pos.pieceBB[static_cast<int>(pos.side)] |= prevBB;

    pos.pieceBB[static_cast<int>(piece)] &= ~currentBB;
    pos.pieceBB[static_cast<int>(piece)] |= prevBB;
    if (move.has_capture_flag())
    {
        Piece capturedPiece = pos.captured.back();
        pos.captured.pop_back();
        if (capturedPiece != Piece::EMPTY)
        {
            pos.pieceBB[static_cast<int>(capturedPiece)] |= currentBB;
            pos.pieceBB[static_cast<int>(pos.get_opposite_side())] |= currentBB;
   
        }

        else if (move.flags() == Flag::EN_PASSANT)
        {
            Square enPasSq = pos.get_enPassant();
            U64 enPasBB = set_bit(enPasSq);
            U64 capturedEnPas = (pos.side == Color::WHITE) ? (enPasBB >> 8) : (enPasBB << 8);
            pos.pieceBB[static_cast<int>(Piece::PAWN)] |= capturedEnPas;
            pos.pieceBB[static_cast<int>(pos.get_opposite_side())] |= capturedEnPas;
        }
    }
    //print_board();
}

















}