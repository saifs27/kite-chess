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

#include "moves.hpp"

namespace Kite {
/*
const Square Move::from() const {
        short mask = 0b0000000000111111;
        int sq = mask & moveValue;
        return static_cast<Square>(sq);


    }
const Square Move::to() const {
        short mask = 0b0000111111000000;
        int sq = (mask & moveValue) >> 6;
        return static_cast<Square>(sq);
    }

const Flag Move::flags() const {
        short mask  = 0b1111000000000000;
        return static_cast<Flag>((moveValue & mask) >> 12);
    }
void Move::set_from(Square from)
{
    short mask  = 0b0000000000111111;
    moveValue &= ~mask;
    moveValue |= static_cast<int>(from);
}

void Move::set_to(Square to)
{
    short mask  = 0b0000111111000000;
    moveValue &= ~mask;
    moveValue |= (static_cast<int>(to) << 6);
}

void Move::set_flags(Flag flags)
{
    short mask  = 0b1111000000000000;
    moveValue &= ~mask;
    moveValue |= (static_cast<int>(flags) << 12);
}
*/

bool Move::has_capture_flag() const {
    Flag flag = flags();
    switch (flag) {
        case Flag::CAPTURE:
            return true;
        case Flag::EN_PASSANT:
            return true;
        case Flag::PROMOTE_ROOK_CAPTURE:
        case Flag::PROMOTE_QUEEN_CAPTURE:
            return true;
        case Flag::PROMOTE_BISHOP_CAPTURE:
        case Flag::PROMOTE_KNIGHT_CAPTURE:
            return true;
        default:
            return false;
    }
}

bool Move::has_promotion_flag() const {
    switch (flags()) {
        case Flag::PROMOTE_BISHOP:
        case Flag::PROMOTE_QUEEN:
        case Flag::PROMOTE_ROOK:
        case Flag::PROMOTE_KNIGHT:
        case Flag::PROMOTE_BISHOP_CAPTURE:
        case Flag::PROMOTE_QUEEN_CAPTURE:
        case Flag::PROMOTE_ROOK_CAPTURE:
        case Flag::PROMOTE_KNIGHT_CAPTURE:
            return true;
        default:
            return false;
    }
}

Piece Move::get_promotion_piece() const {
    Flag flag = flags();
    switch (flag) {
        case Flag::PROMOTE_BISHOP:
        case Flag::PROMOTE_BISHOP_CAPTURE:
            return Piece::BISHOP;
        case Flag::PROMOTE_KNIGHT:
        case Flag::PROMOTE_KNIGHT_CAPTURE:
            return Piece::KNIGHT;
        case Flag::PROMOTE_QUEEN:
        case Flag::PROMOTE_QUEEN_CAPTURE:
            return Piece::QUEEN;
        case Flag::PROMOTE_ROOK:
        case Flag::PROMOTE_ROOK_CAPTURE:
            return Piece::ROOK;
        default:
            return Piece::EMPTY;
    }
}

bool Move::switch_flag_to_capture() {
    Flag flag = flags();
    switch (flag) {
        case Flag::QUIET:
            set_flags(Flag::CAPTURE);
            return true;
        case Flag::PROMOTE_BISHOP:
            set_flags(Flag::PROMOTE_BISHOP_CAPTURE);
            return true;
        case Flag::PROMOTE_KNIGHT:
            set_flags(Flag::PROMOTE_KNIGHT_CAPTURE);
            return true;
        case Flag::PROMOTE_QUEEN:
            set_flags(Flag::PROMOTE_QUEEN_CAPTURE);
            return true;
        case Flag::PROMOTE_ROOK:
            set_flags(Flag::PROMOTE_ROOK_CAPTURE);
            return true;
        case Flag::EN_PASSANT:
            return true;
        default:
            return false;
    }
}

bool Move::is_capture(const U64 opponentBB) const {
    return is_empty(set_bit(to()) & opponentBB);
}

Square Move::get_enPassant_square() const {
    if (flags() != Flag::DOUBLE_PAWN) {
        return Square::A1;
    }

    Color side = (!is_empty(set_bit(from()) & rank::second)) ? Color::WHITE
                                                             : Color::BLACK;
    auto enPasSqWhite = try_offset(from(), 0, 1);
    auto enPasSqBlack = try_offset(from(), 0, -1);
    auto enPasSq = (side == Color::WHITE) ? enPasSqWhite : enPasSqBlack;
    if (enPasSq.has_value()) {
        return enPasSq.value();
    }
    return Square::A1;
}

}  // namespace Kite