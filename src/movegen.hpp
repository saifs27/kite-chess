#pragma once
#include "types.hpp"
#include "position.hpp"


class GenerateMove {
    public:
    void quiet(const Position& pos, const Move move);
    void capture(const Position& pos, const Move move);
    void en_passant(const Position& pos, const Move move);
    void quiet_move(const Position& pos, const Move move);
    void white_pawn_capture(const Position& pos, const Move move);
    void white_pawn(const Position& pos, const Move move);
    void all_moves(const Position& pos, const Move move);
};

enum NormalMoveType {CAPTURE, QUIET, QUIET_CHECK, EVASION, NON_EVASION, LEGAL};