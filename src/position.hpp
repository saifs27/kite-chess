#pragma once
#include <vector>
#include "bitboards.hpp"
#include "types.hpp"
#include <string>
#include "magic.hpp"

#include <iostream>
namespace Smyslov {
class Position { 
    private:
    std::vector<Move> moveHistory; 
    void update_castlingPerm(const Move move);
    public:
    U64 pieceBB[8];
    Color side;
    int enPassant;
    int castlingPerm;
    int fiftyMove;

    Position();                
    U64 colors(Color color) const {return pieceBB[color];};
    U64 pieces(Piece piece) const {return pieceBB[piece];};
    U64 get_bitboard(const Color color, const Piece piece) const;
    U64 get_attacks(const Color color) const;    
    std::optional<Move> uci_to_move(std::string uci);
    void start_position();
    bool can_castle(const Move move) const;
    void make_castle(const Move move);
    void make_en_passant(const Move move);
    bool is_pseudo_legal(const Move move) const;
    bool is_legal(const Move move) const;
    bool make_move(std::string);
    void undo_move();
    void print_board() const;
};

}
