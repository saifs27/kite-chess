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


U64 Position::get_attacks(const Color color) const 
{
    const U64 blockers = colorsBB(color);
    const U64 pawn = pawn_attacks(get_bitboard(color, Piece::PAWN));
    const U64 king = king_attacks(get_bitboard(color, Piece::KING));
    const U64 knight = knight_attacks(get_bitboard(color, Piece::KNIGHT));
    //const U64 rook = rook_attacks(get_bitboard(color, ROOK), blockers);
    const U64 bishop = king_attacks(get_bitboard(color, Piece::BISHOP));
    const U64 queen = knight_attacks(get_bitboard(color, Piece::QUEEN));
    return pawn | king | knight | bishop | queen;
}

std::optional<Move> Position::uci_to_move(std::string uci) 
{
    Move move(Square::EMPTY_SQUARE, Square::EMPTY_SQUARE, Flag::NOFLAG);

    if (uci.compare("0000") == 0) {return move;}

    int from_file = static_cast<int>(uci[0])-97;
    int from_rank = static_cast<int>(uci[1])-49;
    int to_file = static_cast<int>(uci[2])-97;
    int to_rank = static_cast<int>(uci[3])-49;

    int input[4] = {from_file, from_rank, to_file, to_rank};

    for (auto i: input) 
    {
        if (i < 0 || i >= 8) 
        {
            return {};
        }
    }
    move.set_from(static_cast<Square>(from_rank * 8 + from_file));
    move.set_to(static_cast<Square>(to_rank * 8 + to_file));
    Piece pce = get_piece(move.from());
    if (pce == Piece::EMPTY) 
    {
        return {};
    }

    if (pce == Piece::KING && move.from() == Square::E1 && move.to() == Square::G1) 
    {
        move.set_flags(Flag::CASTLE);
    }

    return move;
}


bool Position::can_castle(const Move move) const 
{
    Color color;
    U64 castling_squares;
    const int colorMask = color == Color::WHITE ? 0b0011 : 0b1100;

    switch(move.to()) 
    {
        case Square::G1:
            color = Color::WHITE;
            castling_squares = 0x60ULL;
        case Square::C1:
            color = Color::WHITE;
            castling_squares = 0xeULL;
        case Square::G8:
            color = Color::BLACK;
            castling_squares = 0x6000000000000000ULL;

        case Square::C8:
            color = Color::BLACK;
            castling_squares = 0xe00000000000000ULL;
        default:
            return false;
    }

    Square expected_king_sq = (move.to() == Square::G1 || move.to() == Square::C1) ? Square::E1 : Square::E8;

    U64 king_sq = get_bitboard(color, Piece::KING);

    if (king_sq != set_bit(expected_king_sq)) {return false;}

    if (castlingPerm & colorMask == 0) {return false;}

    const U64 my_pieces = colorsBB(color);
    U64 opponent_attacks = get_attacks(static_cast<Color>((static_cast<int>(color) + 1) % 2));
    U64 blockers = my_pieces | opponent_attacks;

    if ((castling_squares & blockers != 0) || (king_sq & opponent_attacks) != 0) 
    {
        return false;
    }

    return true;
}



void Position::update_castlingPerm(const Move move) {
    const int colorMask = side == Color::WHITE ? 0b0011 : 0b1100;
    Piece piece = get_piece(move.from());
    if (piece == Piece::KING) 
    {
        castlingPerm &= ~colorMask;
    }

    else if (piece == Piece::ROOK) 
    {
        int castling_side;

        switch(move.from()) 
        {
            case Square::A1:
                castling_side = static_cast<int>(Castling::WhiteQueenside) & colorMask;
            case Square::H1:
                castling_side = static_cast<int>(Castling::WhiteKingside) & colorMask;
            case Square::A8:
                castling_side = static_cast<int>(Castling::BlackQueenside) & colorMask;
            case Square::H8:
                castling_side = static_cast<int>(Castling::BlackKingside) & colorMask;
            default:
                castling_side = 0;
        }
        castlingPerm &= ~castling_side;
    }   
}

bool Position::is_pseudo_legal(const Move move) const 
{
    Piece piece = get_piece(move.from());
    if (is_empty(colorsBB(side) & piecesBB(piece) & set_bit(move.from()))) 
    {
        return false;
    }

    U64 attacks;
    U64 blockers = colorsBB(side);


    switch (piece)
    {
        case Piece::KNIGHT:
            attacks = knight_attacks(move.from());
            break;
        case Piece::KING:
            attacks = king_attacks(move.from());
            break;
        case Piece::PAWN:
            attacks = pawn_attacks(move.from()) | pawn_push(move.from(), side) | double_pawn_push(move.from(), side);
            break;
        case Piece::ROOK:
            attacks = rook_attacks(move.from(), blockers);
            break;
        case Piece::BISHOP:
            attacks = bishop_attacks(move.from(), blockers);
            break;
        case Piece::QUEEN:
            attacks = rook_attacks(move.from(), blockers) | bishop_attacks(move.from(), blockers);
            break;
        case Piece::EMPTY:
            return false;
        default:
            return false;
    }

    if (is_empty(attacks & set_bit(move.to()))) 
    {
        return false;
    }

    return true;


}

bool Position::make_move(std::string uci) 
{
    auto input = uci_to_move(uci);

    if (input.has_value() == false) 
    {
        return false;
    }

    Move move = input.value();

    Piece piece = get_piece(move.from());

    if (!is_pseudo_legal(move))
    {
        return false;
    }

    const int colorMask = side == Color::WHITE ? 0b0011 : 0b1100;
    const U64 fromBB = set_bit(move.from());
    const U64 toBB = set_bit(move.to());

    if (fromBB & colorsBB(side) == 0 || fromBB & piecesBB(piece) == 0)
    {
        return false;
    }

    moveHistory.push_back(move);
    pieceBB[static_cast<int>(side)] &= ~fromBB;
    pieceBB[static_cast<int>(side)] |= toBB;

    pieceBB[static_cast<int>(piece)] &= ~fromBB;
    pieceBB[static_cast<int>(piece)] |= toBB;

    update_castlingPerm(move);

    side = (side == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return true;
}

void Position::undo_move() 
{
    Move move = moveHistory.back();
    Piece piece = get_piece(move.from());
    moveHistory.pop_back();

    const U64 prevBB = 0x1ULL << static_cast<int>(move.to());
    const U64 currentBB = 0x1ULL << static_cast<int>(move.from());
    pieceBB[static_cast<int>(side)] &= ~currentBB;
    pieceBB[static_cast<int>(side)] |= prevBB;

    pieceBB[static_cast<int>(piece)] &= ~currentBB;
    pieceBB[static_cast<int>(piece)] |= prevBB;
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


