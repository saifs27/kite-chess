#pragma once
#include "types.hpp"
#include "position.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"

namespace Smyslov::Search {

struct Stack {
    int ply;
    Move current;
    Move excluded;
};

void order_moves(std::vector<Move> moves);
std::vector<Move> filter_illegal_moves(MoveGen& moves);
int quiesce(Position& pos, int alpha, int beta);
int search(Position& pos, int depth);
        



}