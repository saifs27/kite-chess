#pragma once
#include <vector>
#include "types.hpp"
#include "bitboards.hpp"
#include <string>
#include <optional>

class Position { 
    private:
    std::vector<Move> moveHistory; 
    
    public:
    U64 pieceBitBoard[8];
    ColorType side;
    int enPassant;
    int castlingPerm;



    Position();
    Move uci_to_move(std::string uci);
    void startPosition();
    U64 getPieceBB(const ColorType color, const PieceType piece);
    U64 get_attacks(const ColorType color);
    bool can_castle(const Castling castleType);
    bool pseudo_legal(const Move move);
    bool legal(const Move move);
    bool make_move(std::string);
    void undo_move();
};

