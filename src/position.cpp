#include "position.hpp"
namespace Smyslov {
Position::Position() 
{
    set_colorBB(Color::WHITE, 0x0ULL);
    set_colorBB(Color::BLACK, 0x0ULL);
    set_pieceBB(Piece::PAWN, 0x0ULL);
    set_pieceBB(Piece::KNIGHT, 0x0ULL);
    set_pieceBB(Piece::BISHOP, 0x0ULL);
    set_pieceBB(Piece::ROOK, 0x0ULL);
    set_pieceBB(Piece::QUEEN, 0x0ULL);
    set_pieceBB(Piece::KING, 0x0ULL);
    GameState game(0, 15, Piece::EMPTY, Square::A1);
    gameState.push_back(game);
}
void Position::start_position() 
{
    set_colorBB(Color::WHITE, 0xffffULL);
    set_colorBB(Color::BLACK, 0xffff000000000000ULL);
    set_pieceBB(Piece::PAWN,0xff00000000ff00ULL);
    set_pieceBB(Piece::KNIGHT, 0x4200000000000042ULL);
    set_pieceBB(Piece::BISHOP, 0x2400000000000024ULL);
    set_pieceBB(Piece::ROOK, 0x8100000000000081ULL);
    set_pieceBB(Piece::QUEEN, 0x800000000000008ULL);
    set_pieceBB(Piece::KING, 0x1000000000000010ULL);
}

void Position::update_gameState(Move move)
{
    Piece piece = get_piece(move.from());
    auto move50 = (piece == Piece::PAWN || move.has_capture_flag()) ? 0 : fiftyMove() + 1;
    auto castlingRights = update_castlingPerm(move);
    auto enPas = move.get_enPassant_square();
    auto capture = get_piece(move.to());
    GameState board(move50, castlingRights, capture, enPas);
    gameState.push_back(board);
}

void Position::add(Piece piece, Color color, Square addSq)
{
    if (empty_square(addSq))
    {
        pieceBB[static_cast<int>(piece)] |= set_bit(addSq);
        pieceBB[static_cast<int>(color)] |= set_bit(addSq);        
    }
}

void Position::remove(Piece piece, Color color, Square removeSq)
{
    pieceBB[static_cast<int>(piece)] &= ~set_bit(removeSq);
    pieceBB[static_cast<int>(color)] &= ~set_bit(removeSq);
}



bool Position::empty_square(Square sq) const
{
    U64 sqbb = set_bit(sq);
    for (auto bb : pieceBB)
    {
        if (!is_empty(sqbb & bb)) 
        {
            return false;
        }
        
    }
    return true;
}
void Position::push_move(Move move) 
{
    moveHistory.push_back(move);
    if (move.has_capture_flag())
    {
        Piece capturedPiece = get_piece(move.to());
    }
}

U64 Position::get_bitboard(const Color color, const Piece piece) const 
{
    return pieceBB[static_cast<int>(color)] & pieceBB[static_cast<int>(piece)];
}

Piece Position::get_piece(const Square sq) const {
    U64 sqbb = set_bit(sq);
    bool empty = is_empty((colorsBB(Color::WHITE) | colorsBB(Color::BLACK)) & sqbb);

    if (!empty) {
        for (int i = 2; i <= 8; i++)
        {
            if (!is_empty(pieceBB[i] & sqbb)) {
                return static_cast<Piece>(i);
            }
        }
    }

    return Piece::EMPTY;
}

Color Position::check_square_color(const Square sq) const{
    U64 sqBB = set_bit(sq);
    if (!is_empty(pieceBB[static_cast<int>(Color::WHITE)] & sqBB)) return Color::WHITE;
    else if (!is_empty(pieceBB[static_cast<int>(Color::BLACK) & sqBB])) return Color::BLACK;
    else return Color::NONE;
}

Square Position::captured_enPassant(Square enPasSq, Color color) const
{

    int rank_offset = (color == Color::WHITE) ? 1 : -1;
    auto pawn = try_offset(enPasSq, 0, rank_offset);

    return pawn.value_or(Square::A1);

}



U64 Position::get_attacks(const Color color) const 
{
    const U64 blockers = colorsBB(color);
    const U64 pawn = pawn_attacksBB(get_bitboard(color, Piece::PAWN), color);
    const U64 king = king_attacksBB(get_bitboard(color, Piece::KING));
    const U64 knight = knight_attacksBB(get_bitboard(color, Piece::KNIGHT));
    //const U64 rook = rook_attacks(get_bitboard(color, ROOK), blockers);
    const U64 bishop = king_attacksBB(get_bitboard(color, Piece::BISHOP));
    const U64 queen = knight_attacksBB(get_bitboard(color, Piece::QUEEN));
    return pawn | king | knight | bishop | queen;
}



bool Position::can_castle(const Castling castlingSide) const 
{ 
    return !is_empty(static_cast<int>(castlingSide) & castlingPerms());
}



short Position::update_castlingPerm(const Move move) const {
    const short colorMask = (side == Color::WHITE) ? 0b0011 : 0b1100;

    
    Piece piece = get_piece(move.from());
    short currentCastlingPerms = castlingPerms();
    if (colorMask & currentCastlingPerms == 0) {return currentCastlingPerms;}

    if (piece == Piece::KING) 
    {
        return currentCastlingPerms & ~colorMask;
    }

    else if (piece == Piece::ROOK) 
    {
        int castling_side;

        switch(move.from()) 
        {
            case Square::A1:
                castling_side = static_cast<int>(Castling::WhiteQueenside) & colorMask;
                break;
            case Square::H1:
                castling_side = static_cast<int>(Castling::WhiteKingside) & colorMask;
                break;
            case Square::A8:
                castling_side = static_cast<int>(Castling::BlackQueenside) & colorMask;
                break;
            case Square::H8:
                castling_side = static_cast<int>(Castling::BlackKingside) & colorMask;
                break;
            default:
                castling_side = 0;
        }
        return currentCastlingPerms & ~castling_side;
    }
    Square queensideRook = (side == Color::WHITE) ? (Square::A8) : (Square::A1);
    Castling queensideMask = (side == Color::WHITE) ? Castling::BlackQueenside : Castling::WhiteQueenside;
    Square kingsideRook = (side == Color::WHITE) ? (Square::H8) : (Square::H1);
    Castling kingsideMask = (side == Color::WHITE) ? Castling::BlackKingside : Castling::WhiteQueenside;
    if (move.to() == queensideRook && get_piece(move.to()) == Piece::ROOK)
    {
        currentCastlingPerms &= ~static_cast<short>(queensideMask);
    }
    if (move.to() == kingsideRook && get_piece(move.to()) == Piece::ROOK)
    {
        currentCastlingPerms &= ~static_cast<short>(kingsideMask);
    }

    return currentCastlingPerms;   
}


void Position::print_board() const 
{
    std::string printed_board[64];
    U64 check_sq = 0x1ULL;
    Color color;
    for (int i = 0; i < 64; i++) 
    {
        check_sq = 0x1ULL << i;
        std::string symbol = " *";
        printed_board[i] = symbol;
        color = Color::NONE;
        if ((colorsBB(Color::WHITE) & check_sq) != 0) {color = Color::WHITE;}
        else if ((colorsBB(Color::BLACK) & check_sq) != 0) {color = Color::BLACK;}
        else {continue;}

        for (int j = 2; j < 8; j++) {
            Piece piece = static_cast<Piece>(j);
            if ((check_sq & get_bitboard(color, piece)) != 0) 
            {
                switch(piece) 
                {
                    case Piece::PAWN:
                        printed_board[i] = (color == Color::WHITE) ? " P" : " p";
                        break;
                    case Piece::KNIGHT:
                        printed_board[i] = (color == Color::WHITE) ? " N" : " n";
                        break;
                    case Piece::BISHOP:
                        printed_board[i] = (color == Color::WHITE) ? " B" : " b";
                        break;
                    case Piece::ROOK:
                        printed_board[i] = (color == Color::WHITE) ? " R" : " r";
                        break;
                    case Piece::QUEEN:
                        printed_board[i] = (color == Color::WHITE) ? " Q" : " q";
                        break;
                    case Piece::KING:
                        printed_board[i] = (color == Color::WHITE) ? " K" : " k";
                        break;
                    default:
                        break;  
                }
            }
        }
    }
    for (int rank = 7; rank >= 0; rank--) 
    {
        for (int file = 0; file < 8; file++) 
        {
            int sq = rank * 8 + file;
            if (sq % 8 == 0) { std::cout << '\n';}
            std::cout << printed_board[sq];
        }

    }
    std::cout << '\n';


}
}


