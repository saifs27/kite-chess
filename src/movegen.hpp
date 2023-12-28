#pragma once
#include "bitboards.hpp"
#include "position.hpp"

class GenerateMove {
    public:
    void quiet(const Position pos, const int move);
    void capture(const Position pos, const int move);
    void en_passant(const Position pos, const int move);
    void quiet_move(const Position pos, const int move);
    void white_pawn_capture(const Position pos, const int move);
    void white_pawn(const Position pos, const int move);
    void all_moves(const Position pos, const int move);
};

enum NormalMoveType {CAPTURE, QUIET, QUIET_CHECK, EVASION, NON_EVASION, LEGAL};