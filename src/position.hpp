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
namespace Smyslov {
class Position { 
    private:
    Score _score;
    std::array<U64, 8> pieceBB = {0, 0, 0, 0, 0, 0, 0, 0};
    Color _side = Color::WHITE;

    public:
    std::vector<Move> moveHistory = {}; 
    std::vector<GameState> gameState = {};

    public:
    void terminate(GameResult result);
    Color side() const {return _side;};
    void set_side(Color side) { if (side == Color::WHITE || side == Color::BLACK) _side = side;};
    Position();
    Position(std::string fen);
    Position(const Position&) = delete;
    void start_position(); 
    void read_fen(std::string fen);
    void switch_sides() {_side = (side() == Color::WHITE) ? Color::BLACK : Color::WHITE;};
    Color get_opposite_side() const { Color result = (side() == Color::WHITE) ? Color::BLACK : Color::WHITE; return result;}
    Square captured_enPassant(Square enPasSq, Color color) const;
    void shift(Piece, Color color, Move move);
    void add(Piece piece, Color color, Square addSq);    
    void remove(Piece piece, Color color, Square removeSq);
    bool is_empty_square(Square sq) const;
    void push_move(Move move);
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
    void set_colorBB(Color color, U64 bb) {pieceBB[static_cast<int>(color)] = bb;};
    void set_pieceBB(Piece piece, U64 bb) {pieceBB[static_cast<int>(piece)] = bb;};

    Score score() const {return _score;};
    void set_score(Result res) {_score.set_score(res);};
    short update_castlingPerm(const Move move) const;
    std::optional<GameState> new_gameState(Move move) const;

    Square enPassant() const {
        if (!gameState.empty()) 
        {
            return gameState.back().enPassant;
        }
        return Square::A1;
    }

    int fiftyMove() const {
        if (!gameState.empty()) 
        {
            auto lastGameState = gameState.back();
            return lastGameState.fiftyMove;
        }
        return 0;
    }

    short castlingPerms() const {
        if (!gameState.empty()) 
        {
            GameState last_game_state = gameState.back();
            return last_game_state.castlingPerm;
        }
        return 0b0000;
    }




    U64 get_bitboard(const Color color, const Piece piece) const;
    Piece get_piece(const Square sq) const ;
    Color check_square_color(const Square sq) const; // Check if square has white or black piece. Returns Color::NONE if empty.
    U64 get_attacks(const Color color, U64 blockers) const;    
    void print_board() const;
    
    U64 opponent_attacks() const;
    bool is_check() const;
    bool can_castle(const Castling castlingSide) const;

    U64 pieces_attacking_king(Color color) const;
    U64 pin_mask(Color color) const;
    U64 check_mask(Color color) const;


};

}
