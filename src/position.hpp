#pragma once
#include <vector>
#include "bitboards.hpp"
#include "types.hpp"
#include "moves.hpp"
#include <string>


#include <iostream>
namespace Smyslov {
class Position { 
    public:
    std::vector<Move> moveHistory = {}; 
    std::vector<GameState> gameState = {};
    std::vector<Piece> captured = {};
    short update_castlingPerm(const Move move) const;
    
    U64 pieceBB[8];
    Color side;

    Position();    
    void start_position(); 
    void switch_sides() {side = (side == Color::WHITE) ? Color::BLACK : Color::WHITE;};
    Color get_opposite_side() const { Color result = (side == Color::WHITE) ? Color::BLACK : Color::WHITE; return result;}
    Square captured_enPassant(Square enPasSq, Color color) const;
    void push_move(Move move);
    const U64& colorsBB(Color color) const {return pieceBB[static_cast<int>(color)];};
    const U64& piecesBB(Piece piece) const {return pieceBB[static_cast<int>(piece)];};
    void set_colorBB(Color color, U64 bb) {pieceBB[static_cast<int>(color)] = bb;};
    void set_pieceBB(Piece piece, U64 bb) {pieceBB[static_cast<int>(piece)] = bb;};

    Square get_enPassant() const {
        if (!gameState.empty()) {return gameState.back().enPassant;}
        return Square::A1;
    }

    int get_fiftyMove() const {
        if (!gameState.empty()) {return gameState.back().fiftyMove;}
        return 0;
    }

    short get_castlingPerms() const {
        if (!gameState.empty()) {return gameState.back().castlingPerm;}
        return 0b0000;
    }


    U64 get_bitboard(const Color color, const Piece piece) const;
    Piece get_piece(const Square sq) const ;
    Color check_square_color(const Square sq) const; // Check if square has white or black piece. Returns Color::NONE if empty.
    U64 get_attacks(const Color color) const;    
    void print_board() const;

    bool can_castle(const Castling castlingSide) const;

};

}
