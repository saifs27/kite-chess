#include "position.hpp"

Position::Position() {
    pieceBitBoard[WHITE] = 0x0ULL;
    pieceBitBoard[BLACK] = 0x0ULL;
    pieceBitBoard[PAWN] = 0x0ULL;
    pieceBitBoard[KNIGHT] = 0x0ULL;
    pieceBitBoard[BISHOP] = 0x0ULL;
    pieceBitBoard[ROOK] = 0x0ULL;
    pieceBitBoard[QUEEN] = 0x0ULL;
    pieceBitBoard[KING] = 0x0ULL;
}
void Position::startPosition() {
    pieceBitBoard[WHITE] = 0xffffULL;
    pieceBitBoard[BLACK] = 0xffff000000000000ULL;
    pieceBitBoard[PAWN] = 0xff00000000ff00ULL;
    pieceBitBoard[KNIGHT] = 0x4200000000000042ULL;
    pieceBitBoard[BISHOP] = 0x2400000000000024ULL;
    pieceBitBoard[ROOK] = 0x8100000000000081ULL;
    pieceBitBoard[QUEEN] = 0x800000000000008ULL;
    pieceBitBoard[KING] = 0x1000000000000010ULL;
}

U64 Position::getPieceBB(const ColorType color, const PieceType piece) {
    return pieceBitBoard[color] & pieceBitBoard[piece];
}

U64 Position::get_attacks(const ColorType color) {
    const U64 pawn = pawnAttacks(getPieceBB(color, PAWN));
    const U64 king = kingAttacks(getPieceBB(color, KING));
    const U64 knight = knightAttacks(getPieceBB(color, KNIGHT));
    const U64 rook = rookAttacks(getPieceBB(color, ROOK));
    const U64 bishop = kingAttacks(getPieceBB(color, BISHOP));
    const U64 queen = knightAttacks(getPieceBB(color, QUEEN));
    return pawn | king | knight | rook | bishop | queen;
}

Move Position::uci_to_move(std::string uci) {
    Move move;
    move.moveType = NORMAL;
    move.color = side;
    move.piece = EMPTY;
    move.promoted = EMPTY;
    move.from = EMPTY_SQUARE;
    move.to = EMPTY_SQUARE;

    if (uci.compare("0000") == 0) {return move;}




    int from_file = static_cast<int>(uci[0])-97;
    int from_rank = static_cast<int>(uci[1])-49;
    int to_file = static_cast<int>(uci[2])-97;
    int to_rank = static_cast<int>(uci[3])-49;

    int input[4] = {from_file, from_rank, to_file, to_rank};

    for (auto i: input) {
        if (i < 0 || i >= 8) {
            return {};
        }
    }

    move.from = static_cast<Square>(from_rank * 8 + from_file);
    move.to = static_cast<Square>(to_rank * 8 + to_file);
    
    for (int i = 2; i < 8; i++) {
        if ((getPieceBB(move.color, static_cast<PieceType>(i)) & set_bit(move.from)) != 0x0ULL) {
            move.piece = static_cast<PieceType>(i);
            break;
        }
    }
    if (move.piece == EMPTY) {
        return {};
    }

    if (move.piece == KING && move.from == E1 && move.to == G1) {
        move.moveType = CASTLING;
    }

    return move;
}


bool Position::can_castle(const Castling castleType) {
    ColorType color;
    U64 castling_squares;
    U64 king_sq;

    switch(castleType) {
        case WhiteKingside:
            color = WHITE;
            castling_squares = 0x60ULL;
            king_sq = 0x10ULL;
        case WhiteQueenside:
            color = WHITE;
            castling_squares = 0xeULL;
            king_sq = 0x10ULL;
        case BlackKingside:
            color = BLACK;
            castling_squares = 0x6000000000000000ULL;
            king_sq = 0x1000000000000000ULL;

        case BlackQueenside:
            color = BLACK;
            castling_squares = 0xe00000000000000ULL;
            king_sq = 0x1000000000000000ULL;
    }

    const int colorMask = color == WHITE ? 0b0011 : 0b1100;

    if (castlingPerm & colorMask == 0) {
        return false;
    }

    const U64 my_pieces = pieceBitBoard[color];
    U64 opponent_attacks = get_attacks(static_cast<ColorType>((color + 1) % 2));
    U64 blockers = my_pieces | opponent_attacks;

    if ((castling_squares & blockers != 0) || (king_sq & opponent_attacks) != 0) {
        return false;
    }

    return true;
}

bool Position::make_move(std::string uci) {
    auto move = uci_to_move(uci);

    if (move.piece == 0) {
        return false;
    }
    const int colorMask = move.color == WHITE ? 0b0011 : 0b1100;
    const U64 fromBB = 0x1ULL << move.from;
    const U64 toBB = 0x1ULL << move.to;

    if (fromBB & pieceBitBoard[move.color] == 0 || fromBB & pieceBitBoard[move.piece] == 0){
        return false;
    }

    moveHistory.push_back(move);
    pieceBitBoard[move.color] &= ~fromBB;
    pieceBitBoard[move.color] |= toBB;

    pieceBitBoard[move.piece] &= ~fromBB;
    pieceBitBoard[move.piece] |= toBB;

    // update castling permisions if king or rooks move
    if (move.piece == KING) {
        castlingPerm &= ~colorMask;
    }

    else if (move.piece == ROOK) {
        int castling_side;

        switch(move.from) {
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
    return true;
}

void Position::undo_move() {
    Move move = moveHistory.back();
    moveHistory.pop_back();

    const U64 prevBB = 0x1ULL << move.to;
    const U64 currentBB = 0x1ULL << move.from;
    pieceBitBoard[move.color] &= ~currentBB;
    pieceBitBoard[move.color] |= prevBB;

    pieceBitBoard[move.piece] &= ~currentBB;
    pieceBitBoard[move.piece] |= prevBB;
}
