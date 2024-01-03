#pragma once
#include <vector>
#include "bitboards.hpp"
#include <string>
#include <optional>
#include <iostream>

class Position { 
    private:
    std::vector<Move> moveHistory; 
    void update_castlingPerm(const Move move);
    public:
    U64 pieceBB[8];
    Color side;
    int enPassant;
    int castlingPerm;



    Position();
    std::optional<Move> uci_to_move(std::string uci);
    void start_position();
    U64 get_bitboard(const Color color, const Piece piece);
    U64 get_attacks(const Color color);
    bool can_castle(const Move move);
    void make_castle(const Move move);
    void make_en_passant(const Move move);
    bool pseudo_legal(const Move move);
    bool legal(const Move move);
    bool make_move(std::string);
    void undo_move();
    void print_board();
};

