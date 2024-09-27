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

#pragma once
#include "types.hpp"
namespace Kite {

struct Move {
   private:
    Square _from;
    Square _to;
    Flag _flag;

   public:
    Move(Square from, Square to, Flag flag)
        : _from(from), _to(to), _flag(flag) {}

    const Square from() const { return _from; };
    const Square to() const { return _to; };
    const Flag flags() const { return _flag; };

    void set_from(Square from) { _from = from; };
    void set_to(Square to) { _to = to; };
    void set_flags(Flag flags) { _flag = flags; };

    bool has_capture_flag() const;
    bool has_promotion_flag() const;
    bool is_capture(const U64 opponentBB) const;
    Piece get_promotion_piece() const;
    bool switch_flag_to_capture();
    Square get_enPassant_square() const;
};

class GameState {
   private:
    Move _move;
    int _fiftyMove = 0;
    short _castlingPerm = 0b1111;
    Piece _captured = Piece::EMPTY;
    Square _enPassant = Square::A1;
    U64 _posID = 0;

   public:
    GameState(Move move, int fiftyMove, short castling, Piece capture,
              Square enPas, U64 posID)
        : _move(move),
          _enPassant(enPas),
          _captured(capture),
          _castlingPerm(castling),
          _fiftyMove(fiftyMove),
          _posID(posID) {
        if (!is_valid_gameState()) {
            throw std::invalid_argument("invalid gamestate.");
        }
    }

    Move move() const { return _move; }
    int fifty_move() const { return _fiftyMove; }
    short castling_perms() const { return _castlingPerm; }
    Piece captured() const { return _captured; }
    Square en_passant() const { return _enPassant; }
    U64 posID() const { return _posID; }

    void set_move(const Move move) { _move = move; }
    void set_fifty_move(const int half_moves) { _fiftyMove = half_moves; }
    void set_castling_perms(const short castlingPerms) {
        _castlingPerm = castlingPerms;
    }
    void set_captured(Piece captured) { _captured = captured; }
    void set_enPassant(Square sq) { _enPassant = sq; }
    void set_posID(U64 posID) { _posID = posID; }
    bool is_valid_gameState() {
        bool isValidCastling = (_castlingPerm >= 0 && _castlingPerm <= 0b1111);
        bool isValidEnPas = square_in_range(static_cast<int>(_enPassant));
        bool isValidCapture = piece_in_range(static_cast<int>(_captured)) ||
                              _captured == Piece::EMPTY;

        if (move().has_capture_flag() && captured() == Piece::EMPTY)
            return false;

        return isValidCapture && isValidCastling && isValidCapture;
    }
};

/*



More memory efficient but harder to debug. Will switch to this later.

struct Move
{
    private:
    short moveValue;

    public:
    Move(Square from, Square to, Flag flag)
    {
        int start = static_cast<int>(from);
        int end = static_cast<int>(to);
        int flags = static_cast<int>(flag);

        moveValue =  start | (end << 6) | (flags << 12);
    }

    const Square from() const;
    const Square to() const;
    const Flag flags() const;

    void set_from(Square from);
    void set_to(Square to);
    void set_flags(Flag flags);

    bool has_capture_flag() const;
    bool is_capture(const U64 opponentBB) const;
    Piece get_promotion_piece() const;
    bool switch_flag_to_capture();
    Square get_enPassant_square() const;

};
*/

}  // namespace Kite