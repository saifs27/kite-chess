/*
    Kite, a UCI compliant chess engine.
    Copyright (C) 2024  Saif

    Kite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Kite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include "position.hpp"
namespace Kite {
Position::Position(PositionKey& poskeys) : keys(poskeys) {
    set_colorBB(Color::WHITE, 0x0ULL);
    set_colorBB(Color::BLACK, 0x0ULL);
    set_pieceBB(Piece::PAWN, 0x0ULL);
    set_pieceBB(Piece::KNIGHT, 0x0ULL);
    set_pieceBB(Piece::BISHOP, 0x0ULL);
    set_pieceBB(Piece::ROOK, 0x0ULL);
    set_pieceBB(Piece::QUEEN, 0x0ULL);
    set_pieceBB(Piece::KING, 0x0ULL);
    Move null(Square::A1, Square::A1, Flag::QUIET);
    GameState game(null, 0, 0b1111, Piece::EMPTY, Square::A1, 0);
    gameHistory.push_back(game);
}
void Position::start_position() {
    set_colorBB(Color::WHITE, 0xffffULL);
    set_colorBB(Color::BLACK, 0xffff000000000000ULL);
    set_pieceBB(Piece::PAWN, 0xff00000000ff00ULL);
    set_pieceBB(Piece::KNIGHT, 0x4200000000000042ULL);
    set_pieceBB(Piece::BISHOP, 0x2400000000000024ULL);
    set_pieceBB(Piece::ROOK, 0x8100000000000081ULL);
    set_pieceBB(Piece::QUEEN, 0x800000000000008ULL);
    set_pieceBB(Piece::KING, 0x1000000000000010ULL);
    set_side(Color::WHITE);
}

std::optional<GameState> Position::next_game_state(Move move) const {
    Piece piece = get_piece(move.from());
    if (!fiftyMove().has_value()) return {};
    auto move50 = (piece == Piece::PAWN || move.has_capture_flag())
                      ? 0
                      : fiftyMove().value() + 1;
    auto castlingRights = update_castlingPerm(move);
    if (!castlingRights.has_value()) return {};
    auto enPas = move.get_enPassant_square();
    auto capture =
        (move.flags() != Flag::EN_PASSANT) ? get_piece(move.to()) : Piece::PAWN;

    bool isValidCastle = castlingRights >= 0 || castlingRights <= 0b1111;
    bool isValidCapture =
        piece_in_range(static_cast<int>(capture)) || (capture == Piece::EMPTY);
    bool isValidEnPas = square_in_range(static_cast<int>(enPas));

    if (move.has_capture_flag() && capture == Piece::EMPTY) return {};
    
    if (isValidCapture && isValidCastle && isValidEnPas) {
        U64 posID = generate_zobrist();
        GameState board(move, move50, castlingRights.value(), capture, enPas, posID);
        return board;
    }
    return {};
}

void Position::shift(Piece piece, Color color, Move move) {}

void Position::add(Piece piece, Color color, Square addSq) {
    auto pieceInt = static_cast<int>(piece);
    auto colorInt = static_cast<int>(color);
    bool canIndex = piece_in_range(pieceInt) && color_in_range(colorInt);
    // always remove before adding to avoid two pieces on same square.
    if (is_empty_square(addSq) && canIndex) {
        pieceBB[pieceInt] |= set_bit(addSq);
        pieceBB[colorInt] |= set_bit(addSq);
    }
}

void Position::remove(Piece piece, Color color, Square removeSq) {
    int piece_int = static_cast<int>(piece);
    int color_int = static_cast<int>(color);
    bool canIndex = piece_in_range(piece_int) && color_in_range(color_int);
    if (canIndex) {
        pieceBB[piece_int] &= ~set_bit(removeSq);
        pieceBB[color_int] &= ~set_bit(removeSq);
    }
}

bool Position::is_empty_square(Square sq) const {
    U64 sqbb = set_bit(sq);
    for (auto bb : pieceBB) {
        if (!is_empty(sqbb & bb)) {
            return false;
        }
    }
    return true;
}

U64 Position::get_bitboard(const Color color, const Piece piece) const {
    if (color != Color::WHITE && color != Color::BLACK) {
        throw std::invalid_argument(std::to_string(static_cast<int>(piece)));
    }
    return pieceBB.at(static_cast<int>(color)) &
           pieceBB.at(static_cast<int>(piece));
}

Piece Position::get_piece(const Square sq) const {
    U64 sqbb = set_bit(sq);
    bool empty =
        is_empty((colorsBB(Color::WHITE) | colorsBB(Color::BLACK)) & sqbb);

    if (!empty) {
        for (int i = 2; i < 8; i++) {
            if (!is_empty(pieceBB.at(i) & sqbb)) {
                return static_cast<Piece>(i);
            }
        }
    }

    return Piece::EMPTY;
}


Color Position::check_square_color(const Square sq) const {
    U64 sqBB = set_bit(sq);
    if (!is_empty(colorsBB(Color::WHITE) & sqBB)) return Color::WHITE;
    if (!is_empty(colorsBB(Color::BLACK) & sqBB)) return Color::BLACK;
    return Color::NONE;
}

Square Position::captured_enPassant(Square enPasSq, Color color) const {
    int rank_offset = (color == Color::WHITE) ? 1 : -1;
    auto pawn = try_offset(enPasSq, 0, rank_offset);

    return pawn.value_or(Square::A1);
}

std::optional<Square> Position::en_passant() const {
    if (!gameHistory.empty()) {
        auto en_pas = gameHistory.back().en_passant();
        if (square_in_range(static_cast<int>(en_pas))) return en_pas;
    }
    return {};
}

std::optional<int> Position::fiftyMove() const {
    if (!gameHistory.empty()) {
        auto m50 = gameHistory.back().fifty_move();
        if (m50 >= 0) return m50;
    }
    return {};
}

std::optional<short> Position::castlingPerms() const {
    if (!gameHistory.empty()) {
        auto castling = gameHistory.back().castling_perms();
        if (is_valid_castling_perm(castling)) return castling;
    }
    return {};
}

U64 Position::get_attacks(const Color color, U64 blockers) const {
    const U64 pawnAttacks =
        Bitboard::pawn_attacks(get_bitboard(color, Piece::PAWN), color);
    const U64 kingAttacks =
        Bitboard::king_attacks(get_bitboard(color, Piece::KING));
    const U64 knightAttacks =
        Bitboard::knight_attacks(get_bitboard(color, Piece::KNIGHT));

    U64 rooks = get_bitboard(color, Piece::ROOK);
    U64 bishops = get_bitboard(color, Piece::BISHOP);
    U64 queens = get_bitboard(color, Piece::QUEEN);
    U64 rookAttacks = 0x0ULL;
    U64 bishopAttacks = 0x0ULL;
    U64 queenAttacks = 0x0ULL;
    while (!is_empty(rooks)) {
        Square rookSq = Bitboard::pop_lsb(rooks);
        rookAttacks |= Bitboard::rook_attacks(rookSq, blockers);
    }
    while (!is_empty(bishops)) {
        Square bishopSq = Bitboard::pop_lsb(bishops);
        bishopAttacks |= Bitboard::bishop_attacks(bishopSq, blockers);
    }
    while (!is_empty(queens)) {
        Square queenSq = Bitboard::pop_lsb(queens);
        queenAttacks |= (Bitboard::rook_attacks(queenSq, blockers) |
                         Bitboard::bishop_attacks(queenSq, blockers));
    }

    return pawnAttacks | kingAttacks | knightAttacks | rookAttacks |
           bishopAttacks | queenAttacks;
}

bool Position::can_castle(const Castling castlingSide) const {
    if (!castlingPerms().has_value()) return false;
    return !is_empty(static_cast<int>(castlingSide) & castlingPerms().value());
}

std::optional<short> Position::update_castlingPerm(const Move move) const {
    const short colorMask = (side() == Color::WHITE) ? 0b0011 : 0b1100;

    Piece piece = get_piece(move.from());
    if (!castlingPerms().has_value()) return {};
    short currentCastlingPerms = castlingPerms().value();
    short newCastlingPerms = 0;
    U64 initialRookPos = 0x8100000000000081;

    if (currentCastlingPerms == 0) return 0;
    if (move.has_capture_flag()) {
        switch (move.to()) {
            case Square::A1:
                return currentCastlingPerms &
                       ~static_cast<int>(Castling::WhiteQueenside);
                break;
            case Square::H1:
                return currentCastlingPerms &
                       ~static_cast<int>(Castling::WhiteKingside);
                break;
            case Square::A8:
                return currentCastlingPerms &
                       ~static_cast<int>(Castling::BlackQueenside);
                break;
            case Square::H8:
                return currentCastlingPerms &
                       ~static_cast<int>(Castling::BlackKingside);
                break;
            default:
                break;
        }
    }
    if ((colorMask & currentCastlingPerms) == 0) return currentCastlingPerms;

    if (piece == Piece::KING) {
        newCastlingPerms = currentCastlingPerms & ~colorMask;
        if (is_valid_castling_perm(newCastlingPerms)) return newCastlingPerms;
    }

    if (piece == Piece::ROOK) {
        int castling_side;

        switch (move.from()) {
            case Square::A1:
                castling_side =
                    static_cast<int>(Castling::WhiteQueenside) & colorMask;
                break;
            case Square::H1:
                castling_side =
                    static_cast<int>(Castling::WhiteKingside) & colorMask;
                break;
            case Square::A8:
                castling_side =
                    static_cast<int>(Castling::BlackQueenside) & colorMask;
                break;
            case Square::H8:
                castling_side =
                    static_cast<int>(Castling::BlackKingside) & colorMask;
                break;
            default:
                castling_side = 0;
        }
        newCastlingPerms = currentCastlingPerms & ~castling_side;
        if (is_valid_castling_perm(newCastlingPerms)) return newCastlingPerms;
    }
    Square queensideRook =
        (side() == Color::WHITE) ? (Square::A8) : (Square::A1);
    Castling queensideMask = (side() == Color::WHITE)
                                 ? Castling::BlackQueenside
                                 : Castling::WhiteQueenside;
    Square kingsideRook =
        (side() == Color::WHITE) ? (Square::H8) : (Square::H1);
    Castling kingsideMask = (side() == Color::WHITE) ? Castling::BlackKingside
                                                     : Castling::WhiteQueenside;
    if (move.to() == queensideRook && get_piece(move.to()) == Piece::ROOK) {
        currentCastlingPerms &= ~static_cast<short>(queensideMask);
    }
    if (move.to() == kingsideRook && get_piece(move.to()) == Piece::ROOK) {
        currentCastlingPerms &= ~static_cast<short>(kingsideMask);
    }

    if (is_valid_castling_perm(currentCastlingPerms))
        return currentCastlingPerms;

    return {};
}

void Position::print_board() const {
    std::string printed_board[64];
    U64 check_sq = 0x1ULL;
    Color color;
    for (int i = 0; i < 64; i++) {
        check_sq = 0x1ULL << i;
        std::string symbol = " *";
        printed_board[i] = symbol;
        color = Color::WHITE;
        if ((colorsBB(Color::WHITE) & check_sq) != 0) {
            color = Color::WHITE;
        } else if ((colorsBB(Color::BLACK) & check_sq) != 0) {
            color = Color::BLACK;
        } else
            continue;

        for (int j = 2; j < 8; j++) {
            Piece piece = static_cast<Piece>(j);
            if ((check_sq & get_bitboard(color, piece)) != 0) {
                switch (piece) {
                    case Piece::PAWN:
                        printed_board[i] =
                            (color == Color::WHITE) ? " P" : " p";
                        break;
                    case Piece::KNIGHT:
                        printed_board[i] =
                            (color == Color::WHITE) ? " N" : " n";
                        break;
                    case Piece::BISHOP:
                        printed_board[i] =
                            (color == Color::WHITE) ? " B" : " b";
                        break;
                    case Piece::ROOK:
                        printed_board[i] =
                            (color == Color::WHITE) ? " R" : " r";
                        break;
                    case Piece::QUEEN:
                        printed_board[i] =
                            (color == Color::WHITE) ? " Q" : " q";
                        break;
                    case Piece::KING:
                        printed_board[i] =
                            (color == Color::WHITE) ? " K" : " k";
                        break;
                    default:
                        break;
                }
            }
        }
    }
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            int sq = rank * 8 + file;
            if (sq % 8 == 0) {
                std::cout << '\n';
            }
            std::cout << printed_board[sq];
        }
    }
    std::cout << '\n';
}

bool Position::is_check() const {
    U64 kingPos = get_bitboard(side(), Piece::KING);
    U64 attacks = get_attacks(opposite_side(),
                              colorsBB(opposite_side()) | colorsBB(side()));
    return !is_empty(kingPos & attacks);
}

U64 Position::pin_mask(Color color) const {
    // TODO: finish pin mask
    Color op_side = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;

    U64 attacks = get_attacks(color, 0);
    U64 op_blockers = colorsBB(op_side);
    U64 my_pieces = colorsBB(color);
    U64 kingPos = get_bitboard(op_side, Piece::KING);
    Square kingSq = Bitboard::lsb(kingPos);

    U64 op_rooks = get_bitboard(op_side, Piece::ROOK);
    U64 op_bishops = get_bitboard(op_side, Piece::BISHOP);
    U64 op_queens = get_bitboard(op_side, Piece::QUEEN);

    U64 bitboardQR = get_bitboard(op_side, Piece::ROOK) |
                     get_bitboard(op_side, Piece::QUEEN);
    U64 bitboardQB = get_bitboard(op_side, Piece::BISHOP) |
                     get_bitboard(op_side, Piece::QUEEN);

    U64 relevantPieces =
        (Bitboard::rook_attacks(kingSq, op_blockers) & std::move(bitboardQR)) |
        (Bitboard::bishop_attacks(kingSq, op_blockers) & std::move(bitboardQB));

    U64 north = Rays::get_ray_attacks(kingSq, Rays::Direction::NORTH);
    U64 south = Rays::get_ray_attacks(kingSq, Rays::Direction::SOUTH);
    U64 east = Rays::get_ray_attacks(kingSq, Rays::Direction::EAST);
    U64 west = Rays::get_ray_attacks(kingSq, Rays::Direction::WEST);
    U64 nw = Rays::get_ray_attacks(kingSq, Rays::Direction::NW);
    U64 ne = Rays::get_ray_attacks(kingSq, Rays::Direction::NE);
    U64 sw = Rays::get_ray_attacks(kingSq, Rays::Direction::SW);
    U64 se = Rays::get_ray_attacks(kingSq, Rays::Direction::SE);

    std::vector<U64> hv = {north, south, east, west};
    std::vector<U64> diagonals = {nw, ne, sw, se};
    Square attackingPiece;
    U64 hv_mask = 0;
    while (!is_empty(relevantPieces)) {
        Bitboard::pop_lsb(relevantPieces);

        for (auto line : hv) {
            bool isRelevantLine = !is_empty(line & relevantPieces);
            U64 pinned = line & my_pieces;
            Square pinnedSq = Bitboard::pop_lsb(pinned);
            bool isPinned = is_empty(pinned);
            if (isRelevantLine && isPinned) {
                hv_mask |= (line);
            }
        }
    }

    U64 pinned = 0x0ULL;

    if (is_empty(kingPos & attacks)) return 0x0ULL;

    while (!is_empty(my_pieces)) {
        Square sq = Bitboard::pop_lsb(my_pieces);
        attacks = get_attacks(side(), set_bit(sq));
        if (is_empty(kingPos & attacks)) pinned |= set_bit(sq);
    }

    return pinned;
}

U64 Position::pieces_attacking_king(Color color) const {
    Color op_side = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    U64 blockers = colorsBB(Color::WHITE) | colorsBB(Color::BLACK);
    U64 attacks = get_attacks(op_side, blockers);
    U64 kingPos = get_bitboard(color, Piece::KING);

    if (is_empty(attacks & kingPos)) return 0ULL;
    Square kingSq = Bitboard::lsb(kingPos);

    U64 bitboardQR = get_bitboard(op_side, Piece::ROOK) |
                     get_bitboard(op_side, Piece::QUEEN);
    U64 bitboardQB = get_bitboard(op_side, Piece::BISHOP) |
                     get_bitboard(op_side, Piece::QUEEN);

    U64 attackingPieces =
        (Bitboard::knight_attacks(kingPos) &
         get_bitboard(op_side, Piece::KNIGHT)) |
        (Bitboard::pawn_attacks(kingPos, color) &
         get_bitboard(op_side, Piece::PAWN)) |
        (Bitboard::rook_attacks(kingSq, blockers) & std::move(bitboardQR)) |
        (Bitboard::bishop_attacks(kingSq, blockers) & std::move(bitboardQB));

    return attackingPieces;
}

U64 Position::check_mask(Color color) const {
    /*
    For move generation to filter out illegal moves when in check.

    */
    Color op_side = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    U64 blockers = colorsBB(Color::WHITE) | colorsBB(Color::BLACK);
    U64 attacks = get_attacks(op_side, blockers);
    U64 kingPos = get_bitboard(color, Piece::KING);

    if (is_empty(attacks & kingPos)) return 0xffffffffffffffff;

    U64 checkMask = 0;
    Square kingSq = Bitboard::lsb(kingPos);

    U64 bitboardQR = get_bitboard(op_side, Piece::ROOK) |
                     get_bitboard(op_side, Piece::QUEEN);
    U64 bitboardQB = get_bitboard(op_side, Piece::BISHOP) |
                     get_bitboard(op_side, Piece::QUEEN);

    U64 relevantPieces =
        (Bitboard::knight_attacks(kingPos) &
         get_bitboard(op_side, Piece::KNIGHT)) |
        (Bitboard::pawn_attacks(kingPos, color) &
         get_bitboard(op_side, Piece::PAWN)) |
        (Bitboard::rook_attacks(kingSq, blockers) & std::move(bitboardQR)) |
        (Bitboard::bishop_attacks(kingSq, blockers) & std::move(bitboardQB));

    checkMask |= relevantPieces;

    while (!is_empty(relevantPieces)) {
        Square sq = Bitboard::pop_lsb(relevantPieces);
        Piece pieceType = get_piece(sq);
        U64 blockersWithoutKing = blockers & ~(set_bit(sq) | kingPos);

        bool bishopIntersect =
            !is_empty(Bitboard::bishop_attacks(sq, blockersWithoutKing) &
                      set_bit(kingSq));
        bool rookIntersect = !is_empty(
            Bitboard::rook_attacks(sq, blockersWithoutKing) & set_bit(kingSq));

        U64 bishopAttacks = (bishopIntersect)
                                ? Bitboard::bishop_attacks(sq, blockers) &
                                      Bitboard::bishop_attacks(kingSq, blockers)
                                : 0x0ULL;
        U64 rookAttacks = (rookIntersect)
                              ? Bitboard::rook_attacks(sq, blockers) &
                                    Bitboard::rook_attacks(kingSq, blockers)
                              : 0x0ULL;
        U64 queenAttacks;
        switch (pieceType) {
            case Piece::BISHOP:
                checkMask |= bishopAttacks;
                break;
            case Piece::ROOK:
                checkMask |= rookAttacks;
                break;
            case Piece::QUEEN:
                queenAttacks = rookAttacks | bishopAttacks;
                checkMask |= queenAttacks;
                break;
            default:
                break;
        }
    }

    return checkMask;
}

Position::Position(std::string fen, PositionKey& poskeys): keys(poskeys) {
    set_fen(fen);
}

void Position::set_fen(std::string fen) {
    _score.set_score(Result::EMPTY);
    set_colorBB(Color::WHITE, 0x0ULL);
    set_colorBB(Color::BLACK, 0x0ULL);
    set_pieceBB(Piece::PAWN, 0x0ULL);
    set_pieceBB(Piece::KNIGHT, 0x0ULL);
    set_pieceBB(Piece::BISHOP, 0x0ULL);
    set_pieceBB(Piece::ROOK, 0x0ULL);
    set_pieceBB(Piece::QUEEN, 0x0ULL);
    set_pieceBB(Piece::KING, 0x0ULL);

    auto f = split_fen(fen, ' ');
    if (f.size() != 6) {
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

    for (auto i : fen_position) {
        switch (i) {
            case 'Q':
            case 'q':
                piece = Piece::QUEEN;
                color = (i == 'Q') ? Color::WHITE : Color::BLACK;
                break;
            case 'R':
            case 'r':
                piece = Piece::ROOK;
                color = (i == 'R') ? Color::WHITE : Color::BLACK;
                break;
            case 'K':
            case 'k':
                piece = Piece::KING;
                color = (i == 'K') ? Color::WHITE : Color::BLACK;
                break;
            case 'N':
            case 'n':
                piece = Piece::KNIGHT;
                color = (i == 'N') ? Color::WHITE : Color::BLACK;
                break;
            case 'B':
            case 'b':
                piece = Piece::BISHOP;
                color = (i == 'B') ? Color::WHITE : Color::BLACK;
                break;
            case 'P':
            case 'p':
                piece = Piece::PAWN;
                color = (i == 'P') ? Color::WHITE : Color::BLACK;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = Piece::EMPTY;
                color = Color::NONE;
                file += static_cast<int>(i) - '0';
                sq = rank * 8 + file;
                break;
            case '/':
                piece = Piece::EMPTY;
                color = Color::NONE;
                rank--;
                file = 0;
                break;
            default:
                throw std::invalid_argument(
                    "Invalid FEN, could not parse position");
        }

        if (piece != Piece::EMPTY && color != Color::NONE) {
            sq = rank * 8 + file;
            add(piece, color, static_cast<Square>(sq));
            file++;
        }
    }

    for (auto i : fen_castling) {
        switch (i) {
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
            case '-':
                break;
            default:
                throw std::invalid_argument(
                    "Invalid FEN, could not parse castling flags");
        }
    }

    if (castling > 0b1111)
        throw std::invalid_argument(
            "Invalid FEN. Could not parse castling flags");

    int enPasFile = 10;
    int enPasRank = 10;
    bool noEnPas = false;
    for (auto i : fen_en_passant) {
        if (noEnPas) break;
        switch (i) {
            case '-':
                noEnPas = true;
                break;
            case 'a':
                enPasFile = 0;
                break;
            case 'b':
                enPasFile = 1;
                break;
            case 'c':
                enPasFile = 2;
                break;
            case 'd':
                enPasFile = 3;
                break;
            case 'e':
                enPasFile = 4;
                break;
            case 'f':
                enPasFile = 5;
                break;
            case 'g':
                enPasFile = 6;
                break;
            case 'h':
                enPasFile = 7;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                enPasRank = i - '0' - 1;
                break;
            default:
                throw std::invalid_argument(
                    "Invalid FEN, could not parse en passant square");
        }
    }

    if ((enPasFile >= 8 || enPasRank >= 8) && !noEnPas) {
        throw std::invalid_argument(
            "Invalid FEN, could not parse en passant square");
    }

    int halfmoves = 0;
    for (auto i : fen_halfmoves) {
        int digit = i - '0';
        if (digit > 9 || digit < 0) {
            throw std::invalid_argument(
                "Invalid FEN. Could not parse halfmoves.");
        }
        halfmoves *= 10;
        halfmoves += digit;
    }

    Square enPasSquare = (!noEnPas)
                             ? static_cast<Square>(enPasFile + enPasRank * 8)
                             : Square::A1;
    if (!square_in_range(static_cast<int>(enPasSquare)))
        throw std::invalid_argument(
            "Invalid FEN. Could not parse en passant square.");

    Color side = (fen_side == "w") ? Color::WHITE : Color::BLACK;
    set_side(side);
    Move nullMove(Square::A1, Square::A1, Flag::QUIET);
    U64 posID = generate_zobrist();
    GameState game(nullMove, halfmoves, castling, Piece::EMPTY, enPasSquare, 0);
    gameHistory.push_back(game);
}

U64 Position::generate_zobrist() const {
    U64 piece_key = 0;
    U64 en_pas_key = keys.get_en_pas_key(
        get_file(en_passant().value_or(Square::EMPTY_SQUARE)));
    U64 castling_key = keys.get_castling_key(castlingPerms().value_or(0));
    U64 side_key = keys.get_side_key(side());

    U64 nonempty_squares =
        colorsBB(Color::WHITE) | colorsBB(Color::BLACK);
    U64 sq = 1;
    int sq_idx = 0;

    Piece piece;
    Color piece_color = Color::NONE;
    Square square;
    for (int i = 0; i < 64; i++) {
        if (sq_idx >= 64) break;
        if ((nonempty_squares & sq) != 0) {
            piece = get_piece(static_cast<Square>(sq_idx));
            piece_color = check_square_color(static_cast<Square>(sq_idx));
            square = static_cast<Square>(i);
            piece_key ^= keys.get_piece_key(square, piece_color, piece);
        }
        sq <<= 1;
        sq_idx += 1;
    }

    return piece_key ^ en_pas_key ^ castling_key ^ side_key;
}

}  // namespace Kite
