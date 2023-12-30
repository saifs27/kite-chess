#pragma once
#include <vector>
#include "types.hpp"
#include "bitboards.hpp"
#include <string>
#include <optional>
#include <iostream>

class Position { 
    private:
    std::vector<Move> moveHistory; 
    void update_castlingPerm(const Move move);
    public:
    U64 pieceBitBoard[8];
    ColorType side;
    int enPassant;
    int castlingPerm;



    Position();
    std::optional<Move> uci_to_move(std::string uci);
    void startPosition();
    U64 getPieceBB(const ColorType color, const PieceType piece);
    U64 get_attacks(const ColorType color);
    bool can_castle(const Move move);
    bool pseudo_legal(const Move move);
    bool legal(const Move move);
    bool make_move(std::string);
    void undo_move();
    void print_board();
};

