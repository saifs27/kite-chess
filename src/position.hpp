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
#include <vector>
#include "bitboards.hpp"
#include "types.hpp"
#include "moves.hpp"
#include <string>
#include "rays.hpp"
#include <array>
#include <limits>
#include <algorithm>
#include <iostream>

namespace Kite {
class Position { 
    private:
    Score _score;
    std::array<U64, 8> pieceBB = {0, 0, 0, 0, 0, 0, 0, 0};
    Color _side = Color::WHITE;

    public:
    std::vector<GameState> gameHistory = {};

    public:
    Position();
    Position(std::string fen);
    void start_position();
    void set_fen(std::string fen);

    void read_fen(std::string fen);

    Color side() const {return _side;};
    Color opposite_side() const { Color result = (side() == Color::WHITE) ? Color::BLACK : Color::WHITE; return result;}
    void set_side(Color side) { if (side == Color::WHITE || side == Color::BLACK) _side = side;};
    void switch_sides() {_side = (side() == Color::WHITE) ? Color::BLACK : Color::WHITE;};
    Score score() const {return _score;};
    void shift(Piece, Color color, Move move);
    void add(Piece piece, Color color, Square addSq);    
    void remove(Piece piece, Color color, Square removeSq);   
    void set_colorBB(Color color, U64 bb) {pieceBB[static_cast<int>(color)] = bb;};
    void set_pieceBB(Piece piece, U64 bb) {pieceBB[static_cast<int>(piece)] = bb;};
    void set_score(Result res) {_score.set_score(res);};
    
    Square captured_enPassant(Square enPasSq, Color color) const;
    bool is_empty_square(Square sq) const;
    const U64 colorsBB(Color color) const 
    {
        if (color_in_range(static_cast<int>(color))) return pieceBB[static_cast<int>(color)];
        else return 0;
    }
    const U64 piecesBB(Piece piece) const 
    {
        if (piece_in_range(static_cast<int>(piece))) return pieceBB[static_cast<int>(piece)];
        else return 0;
    }


    
    
    std::optional<short> update_castlingPerm(const Move move) const;
    std::optional<GameState> next_game_state(Move move) const;

    std::optional<Square> en_passant() const;
    std::optional<int> fiftyMove() const;
    std::optional<short> castlingPerms() const;
    U64 get_bitboard(const Color color, const Piece piece) const;
    Piece get_piece(const Square sq) const ;
    Color check_square_color(const Square sq) const; // Check if square has white or black piece. Returns Color::NONE if empty.
    U64 get_attacks(const Color color, U64 blockers) const;    
    void print_board() const;
    
    bool is_check() const;
    bool can_castle(const Castling castlingSide) const;

    U64 pieces_attacking_king(Color color) const;
    U64 pin_mask(Color color) const;
    U64 check_mask(Color color) const;


};

}
