#pragma once
#include "types.hpp"
#include "position.hpp"

namespace Smyslov {

struct MoveGen
{
    std::vector<Move> moveList;
    Smyslov::Position& pos;
    void generate_king_moves();
    void generate_moves(Piece piece);
    void add_quiet(const Move move);
    void add_capture(const Move move);
    void add_en_passant(const Move move);
    void add_quiet_move(const Move move);
    void add_white_pawn_capture(const Move move);
    void add_white_pawn(const Move move);
    void add_all_moves(const Move move);


};







enum NormalMoveType {CAPTURE, QUIET, QUIET_CHECK, EVASION, NON_EVASION, LEGAL};
}