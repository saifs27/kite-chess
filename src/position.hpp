#pragma once
#include <vector>
#include "bitboards.hpp"
#include "types.hpp"
#include <string>


#include <iostream>
namespace Smyslov {
class Position { 
    public:
    std::vector<Move> moveHistory; 
    std::vector<GameState> gameState;
    std::vector<Piece> captured;
    void update_castlingPerm(const Move move);
    
    U64 pieceBB[8];
    Color side;
    Square enPassant = Square::A1;
    int castlingPerm;
    int fiftyMove;

    Position();    
    void start_position(); 
    void switch_sides() {side = (side == Color::WHITE) ? Color::BLACK : Color::WHITE;};
    Color get_opposite_side() const { Color result = (side == Color::WHITE) ? Color::BLACK : Color::WHITE; return result;}
    Square captured_enPassant(Square enPasSq) const;
    void push_move(Move move);
    Move pop_move();
    Piece pop_captureList();
    const U64& colorsBB(Color color) const {return pieceBB[static_cast<int>(color)];};
    const U64& piecesBB(Piece piece) const {return pieceBB[static_cast<int>(piece)];};
    void set_colorBB(Color color, U64 bb) {pieceBB[static_cast<int>(color)] = bb;};
    void set_pieceBB(Piece piece, U64 bb) {pieceBB[static_cast<int>(piece)] = bb;};


    U64 get_bitboard(const Color color, const Piece piece) const;
    Piece get_piece(const Square sq) const ;
    U64 get_attacks(const Color color) const;    
    std::optional<Move> uci_to_move(std::string uci);
    void print_board() const;

    bool can_castle(const Move move) const;
    bool is_pseudo_legal(const Move move) const;
    bool is_legal(const Move move) const;
};

}
