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


GameState Position::new_gameState(Move move) const
{
    Piece piece = get_piece(move.from());
    auto move50 = (piece == Piece::PAWN || move.has_capture_flag()) ? 0 : fiftyMove() + 1;
    auto castlingRights = update_castlingPerm(move);
    auto enPas = move.get_enPassant_square();
    auto capture = (move.flags() != Flag::EN_PASSANT) ? get_piece(move.to()) : Piece::PAWN;
    GameState board(move50, castlingRights, capture, enPas);
    return board;
}



void Position::add(Piece piece, Color color, Square addSq)
{
    // always remove before adding to avoid two pieces on same square.
    if (is_empty_square(addSq))
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



bool Position::is_empty_square(Square sq) const
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



U64 Position::get_attacks(const Color color, U64 blockers) const 
{
    const U64 pawnAttacks = pawn_attacksBB(get_bitboard(color, Piece::PAWN), color);
    const U64 kingAttacks = king_attacksBB(get_bitboard(color, Piece::KING));
    const U64 knightAttacks = knight_attacksBB(get_bitboard(color, Piece::KNIGHT));

    U64 rooks = get_bitboard(color, Piece::ROOK);
    U64 bishops = get_bitboard(color, Piece::BISHOP);
    U64 queens = get_bitboard(color, Piece::QUEEN);
    U64 rookAttacks = 0x0ULL;
    U64 bishopAttacks = 0x0ULL;
    U64 queenAttacks = 0x0ULL;
    while (!is_empty(rooks))
    {
        Square rookSq = pop_lsb(rooks);
        rookAttacks |= rook_attacks(rookSq, blockers);
    }
    while (!is_empty(bishops))
    {
        Square bishopSq = pop_lsb(bishops);
        bishopAttacks |= bishop_attacks(bishopSq, blockers);
    }
    while (!is_empty(queens))
    {
        Square queenSq = pop_lsb(queens);
        queenAttacks |= (rook_attacks(queenSq, blockers) | bishop_attacks(queenSq, blockers));
    }

    return pawnAttacks | kingAttacks | knightAttacks | bishopAttacks | queenAttacks;
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


bool Position::is_check() const
{
    U64 kingPos = get_bitboard(side, Piece::KING);
    U64 attacks = get_attacks(get_opposite_side(), colorsBB(side));
    return !is_empty(kingPos & attacks);
}


U64 Position::pinned_pieces() const
{
    U64 attacks = get_attacks(get_opposite_side(), 0);
    U64 my_pieces = colorsBB(side);
    U64 kingPos = get_bitboard(side, Piece::KING);
    U64 pinned = 0x0ULL;

    if (is_empty(kingPos & attacks)) return 0x0ULL;
    
    while (!is_empty(my_pieces))
    {
        Square sq = pop_lsb(my_pieces);
        attacks = get_attacks(side, set_bit(sq));
        if (is_empty(kingPos & attacks)) pinned |= set_bit(sq);
    }

    return pinned;
}

U64 Position::potential_checks() const
{
    Color color = get_opposite_side();
    U64 all_attacks = 0x0ULL;
    U64 rooks = get_bitboard(color, Piece::ROOK);
    U64 bishops = get_bitboard(color, Piece::BISHOP);
    U64 queens = get_bitboard(color, Piece::QUEEN);
    U64 rookAttacks = 0x0ULL;
    U64 bishopAttacks = 0x0ULL;
    U64 queenAttacks = 0x0ULL;
    U64 kingPos = get_bitboard(side, Piece::KING);
    while (!is_empty(rooks))
    {
        Square rookSq = pop_lsb(rooks);
        rookAttacks = rook_attacks(rookSq, 0);
        if (!is_empty(rookAttacks & kingPos)) all_attacks |= rookAttacks;
    }
    while (!is_empty(bishops))
    {
        Square bishopSq = pop_lsb(bishops);
        bishopAttacks = bishop_attacks(bishopSq, 0);
        if (!is_empty(bishopAttacks & kingPos)) all_attacks |= bishopAttacks;
    }
    while (!is_empty(queens))
    {
        Square queenSq = pop_lsb(queens);
        queenAttacks |= (rook_attacks(queenSq, 0) | bishop_attacks(queenSq, 0));
        if (!is_empty(queenAttacks & kingPos)) all_attacks |= queenAttacks;
    }
    return all_attacks;
}


Position::Position(std::string fen)
{
    set_colorBB(Color::WHITE, 0x0ULL);
    set_colorBB(Color::BLACK, 0x0ULL);
    set_pieceBB(Piece::PAWN, 0x0ULL);
    set_pieceBB(Piece::KNIGHT, 0x0ULL);
    set_pieceBB(Piece::BISHOP, 0x0ULL);
    set_pieceBB(Piece::ROOK, 0x0ULL);
    set_pieceBB(Piece::QUEEN, 0x0ULL);
    set_pieceBB(Piece::KING, 0x0ULL);


    auto f = split_fen(fen, ' ');
    if (f.size() != 6)
    {
        throw std::invalid_argument("Invalid FEN");
    }

    auto fen_position = f.at(0);
    auto fen_side = f.at(1);
    auto fen_castling = f.at(2);
    auto fen_en_passant = f.at(3);
    auto fen_halfmoves = f.at(4);
    auto fen_fullmoves = f.at(5);

    short castling = 0;
    Square en_passant_sq = Square::A1;
    int file = File::A;
    int rank = Rank::Eighth;
    int sq = rank * 8 + file;
    Piece piece;
    Color color;
    
    for (auto i : fen_position)
    {
        switch (i)
        {
            case 'Q': case 'q':
                piece = Piece::QUEEN;
                color = (i == 'Q') ? Color::WHITE : Color::BLACK;
                break;
            case 'R': case 'r':
                piece = Piece::ROOK;
                color = (i == 'R') ? Color::WHITE : Color::BLACK;
                break;
            case 'K': case 'k':
                piece = Piece::KING;
                color = (i == 'K') ? Color::WHITE : Color::BLACK;
                break;
            case 'N': case 'n':
                piece = Piece::KNIGHT;
                color = (i == 'N') ? Color::WHITE : Color::BLACK;
                break;
            case 'B': case 'b':
                piece = Piece::BISHOP;
                color = (i == 'B') ? Color::WHITE : Color::BLACK;
                break;
            case 'P': case 'p':
                piece = Piece::PAWN;
                color = (i == 'P') ? Color::WHITE : Color::BLACK;
                break;
            case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
                piece = Piece::EMPTY; color = Color::NONE;
                file += static_cast<int>(i) - '0';
                sq = rank * 8 + file;
                break; 
            case '/':
                piece = Piece::EMPTY; color = Color::NONE;
                rank--;
                file = 0;
                break;
            default:
                throw std::invalid_argument("Invalid FEN, could not parse position");

        }

        if (piece != Piece::EMPTY && color != Color::NONE)
        {
     
            sq = rank * 8 + file;
            add(piece, color, static_cast<Square>(sq));
            file++;
        }
    }


    for (auto i: fen_castling)
    {
        switch (i)
        {
            case 'K':
            castling |= static_cast<short>(Castling::WhiteKingside);
            break;
            case 'Q':
            castling |= static_cast<short>(Castling::WhiteQueenside);
            break;
            case 'k':
            castling |= static_cast<short>(Castling::BlackKingside);
            break;
            case 'q':
            castling |= static_cast<short>(Castling::BlackQueenside);
            break;
            default:
            throw std::invalid_argument("Invalid FEN, could not parse castling flags");
        }
    }

    
    int enPasFile = 10;
    int enPasRank = 10;
    bool noEnPas = false;
    for (auto i : fen_en_passant)
    {
        if (noEnPas) break;
        switch (i)
        {
            case '-':
            noEnPas = true;
            break;
            case 'a':
                enPasFile = 0; break;
            case 'b':
                enPasFile = 1; break;
            case 'c':
                enPasFile = 2; break;
            case 'd':
                enPasFile = 3; break;
            case 'e':
                enPasFile = 4; break;
            case 'f':
                enPasFile = 5; break;
            case 'g':
                enPasFile = 6; break;
            case 'h':
                enPasFile = 7; break;
            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8':
                enPasRank = i - '0' - 1; break;
            default:
                throw std::invalid_argument("Invalid FEN, could not parse en passant square");
        }

        
    }

    if ((enPasFile >= 8 || enPasRank >= 8) && !noEnPas)
    {
        throw std::invalid_argument("Invalid FEN, could not parse en passant square");
    }

    int halfmoves = 0;
    for (auto i : fen_halfmoves)
    {
        int digit = i - '0';
        if (digit > 9 || digit < 0)
        {
            throw std::invalid_argument("Invalid FEN. Could not parse halfmoves.");
        }
        halfmoves *= 10;
        halfmoves += digit;
    }
    

    Square enPasSquare = static_cast<Square>(enPasFile + enPasRank * 8);
    side = (fen_side == "w") ? Color::WHITE : Color::BLACK;
    GameState game(halfmoves, castling, Piece::EMPTY, enPasSquare);
    gameState.push_back(game);
}

}


