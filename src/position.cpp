#include "position.hpp"
namespace Smyslov {
Position::Position() 
{
    pieceBB[WHITE] = 0x0ULL;
    pieceBB[BLACK] = 0x0ULL;
    pieceBB[PAWN] = 0x0ULL;
    pieceBB[KNIGHT] = 0x0ULL;
    pieceBB[BISHOP] = 0x0ULL;
    pieceBB[ROOK] = 0x0ULL;
    pieceBB[QUEEN] = 0x0ULL;
    pieceBB[KING] = 0x0ULL;
}
void Position::start_position() 
{
    pieceBB[WHITE] = 0xffffULL;
    pieceBB[BLACK] = 0xffff000000000000ULL;
    pieceBB[PAWN] = 0xff00000000ff00ULL;
    pieceBB[KNIGHT] = 0x4200000000000042ULL;
    pieceBB[BISHOP] = 0x2400000000000024ULL;
    pieceBB[ROOK] = 0x8100000000000081ULL;
    pieceBB[QUEEN] = 0x800000000000008ULL;
    pieceBB[KING] = 0x1000000000000010ULL;
}

U64 Position::get_bitboard(const Color color, const Piece piece) const 
{
    return pieceBB[color] & pieceBB[piece];
}

U64 Position::get_attacks(const Color color) const 
{
    const U64 pawn = pawn_attacks(get_bitboard(color, PAWN));
    const U64 king = king_attacks(get_bitboard(color, KING));
    const U64 knight = knight_attacks(get_bitboard(color, KNIGHT));
    const U64 rook = rook_attacks(get_bitboard(color, ROOK));
    const U64 bishop = king_attacks(get_bitboard(color, BISHOP));
    const U64 queen = knight_attacks(get_bitboard(color, QUEEN));
    return pawn | king | knight | rook | bishop | queen;
}

std::optional<Move> Position::uci_to_move(std::string uci) 
{
    Move move(EMPTY_SQUARE, EMPTY_SQUARE, Piece::EMPTY, side);

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

    move.from = static_cast<Square>(from_rank * 8 + from_file);
    move.to = static_cast<Square>(to_rank * 8 + to_file);
    
    for (int i = 2; i < 8; i++) {
        if ((get_bitboard(move.color, static_cast<Piece>(i)) & set_bit(move.from)) != 0x0ULL) 
        {
            move.piece = static_cast<Piece>(i);
            break;
        }
    }
    if (move.piece == EMPTY) 
    {
        return {};
    }

    if (move.piece == KING && move.from == E1 && move.to == G1) 
    {
        move.moveType = CASTLING;
    }

    return move;
}


bool Position::can_castle(const Move move) const 
{
    Color color;
    U64 castling_squares;
    const int colorMask = color == WHITE ? 0b0011 : 0b1100;

    switch(move.to) 
    {
        case G1:
            color = WHITE;
            castling_squares = 0x60ULL;
        case C1:
            color = WHITE;
            castling_squares = 0xeULL;
        case G8:
            color = BLACK;
            castling_squares = 0x6000000000000000ULL;

        case C8:
            color = BLACK;
            castling_squares = 0xe00000000000000ULL;
        default:
            return false;
    }

    Square expected_king_sq = (move.to == G1 || move.to == C1) ? E1 : E8;

    U64 king_sq = get_bitboard(color, KING);

    if (king_sq != set_bit(expected_king_sq)) {return false;}

    if (castlingPerm & colorMask == 0) {return false;}

    const U64 my_pieces = pieceBB[color];
    U64 opponent_attacks = get_attacks(static_cast<Color>((color + 1) % 2));
    U64 blockers = my_pieces | opponent_attacks;

    if ((castling_squares & blockers != 0) || (king_sq & opponent_attacks) != 0) 
    {
        return false;
    }

    return true;
}



void Position::update_castlingPerm(const Move move) {
    const int colorMask = move.color == WHITE ? 0b0011 : 0b1100;
    if (move.piece == KING) 
    {
        castlingPerm &= ~colorMask;
    }

    else if (move.piece == ROOK) 
    {
        int castling_side;

        switch(move.from) 
        {
            case A1:
                castling_side = WhiteQueenside & colorMask;
            case H1:
                castling_side = WhiteKingside & colorMask;
            case A8:
                castling_side = BlackQueenside & colorMask;
            case H8:
                castling_side = BlackKingside & colorMask;
            default:
                castling_side = 0;
        }
        castlingPerm &= ~castling_side;
    }   
}

bool Position::is_pseudo_legal(const Move move) const 
{
    if ((pieceBB[move.color] & pieceBB[move.piece] & set_bit(move.from)) == 0x0ULL) 
    {
        return false;
    }

    U64 attacks;

    switch (move.piece)
    {
        case KNIGHT:
            attacks = knight_attacks(move.from);
            break;
        case KING:
            attacks = king_attacks(move.from);
            break;
        case PAWN:
            attacks = pawn_attacks(move.from) | pawn_push(move) | double_pawn_push(move);
            break;
        case ROOK:
            attacks = rook_attacks(move.from);
            break;
        case BISHOP:
            attacks = bishop_attacks(move.from);
            break;
        case QUEEN:
            attacks = rook_attacks(move.from) | bishop_attacks(move.from);
            break;
        case EMPTY:
            return false;
    }

    if ((attacks & set_bit(move.to)) == 0x0ULL) 
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

    if (!is_pseudo_legal(move))
    {
        return false;
    }

    const int colorMask = move.color == WHITE ? 0b0011 : 0b1100;
    const U64 fromBB = set_bit(move.from);
    const U64 toBB = set_bit(move.to);

    if (fromBB & pieceBB[move.color] == 0 || fromBB & pieceBB[move.piece] == 0)
    {
        return false;
    }

    moveHistory.push_back(move);
    pieceBB[move.color] &= ~fromBB;
    pieceBB[move.color] |= toBB;

    pieceBB[move.piece] &= ~fromBB;
    pieceBB[move.piece] |= toBB;

    update_castlingPerm(move);

    side = (side == WHITE) ? BLACK : WHITE;
    return true;
}

void Position::undo_move() 
{
    Move move = moveHistory.back();
    moveHistory.pop_back();

    const U64 prevBB = 0x1ULL << move.to;
    const U64 currentBB = 0x1ULL << move.from;
    pieceBB[move.color] &= ~currentBB;
    pieceBB[move.color] |= prevBB;

    pieceBB[move.piece] &= ~currentBB;
    pieceBB[move.piece] |= prevBB;
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
        color = NONE;
        if ((pieceBB[WHITE] & check_sq) != 0) {color = WHITE;}
        else if ((pieceBB[BLACK] & check_sq) != 0) {color = BLACK;}
        else {continue;}

        for (int j = 2; j < 8; j++) {
            Piece piece = static_cast<Piece>(j);
            if ((check_sq & get_bitboard(color, piece)) != 0) 
            {
                switch(piece) 
                {
                    case PAWN:
                        printed_board[i] = (color == WHITE) ? " P" : " p";
                        break;
                    case KNIGHT:
                        printed_board[i] = (color == WHITE) ? " N" : " n";
                        break;
                    case BISHOP:
                        printed_board[i] = (color == WHITE) ? " B" : " b";
                        break;
                    case ROOK:
                        printed_board[i] = (color == WHITE) ? " R" : " r";
                        break;
                    case QUEEN:
                        printed_board[i] = (color == WHITE) ? " Q" : " q";
                        break;
                    case KING:
                        printed_board[i] = (color == WHITE) ? " K" : " k";
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
