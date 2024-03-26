/*
    Kite, a UCI compliant chess engine.
    Copyright (C) 2024  Saif

    Kite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Kite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/


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