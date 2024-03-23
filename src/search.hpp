#pragma once
#include "types.hpp"
#include "position.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"

namespace Kite::Search {

struct Stack {
    int ply;
    Move current;
    Move excluded;
};

void order_moves(std::vector<Move> moves);
std::vector<Move> filter_illegal_moves(MoveGen& moves);
int quiesce(Position& pos, MoveGen& moves, int alpha, int beta); // Needed to avoid the horizon effect
int alpha_beta(Position& pos, MoveGen& moves, int alpha, int beta, int depthLeft);
int search(Position& pos, int depth);
        



}