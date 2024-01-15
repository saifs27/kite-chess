#pragma once
#include "types.hpp"
#include "position.hpp"

namespace Smyslov {
class MoveGen {
    Position& m_position;
    public:
    MoveGen(Position& position) : m_position(position) {};
    void quiet(const Move move);
    void capture(const Move move);
    void en_passant(const Move move);
    void quiet_move(const Move move);
    void white_pawn_capture(const Move move);
    void white_pawn(const Move move);
    void all_moves(const Move move);
};

enum NormalMoveType {CAPTURE, QUIET, QUIET_CHECK, EVASION, NON_EVASION, LEGAL};
}