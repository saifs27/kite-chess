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


#include "search.hpp"

namespace Kite::Search {

void order_moves(const Position& pos, std::vector<Move> moves)
{
    for (Move move : moves)
    {
        int moveScore = 0;
        Piece fromPieceType = pos.get_piece(move.from());

        if (!move.has_capture_flag()) continue;
        
        Piece capturePieceType = pos.get_piece(move.to());
        moveScore = 10 * get_piece_value(capturePieceType) - get_piece_value(fromPieceType);
    }
}

std::vector<Move> filter_illegal_moves(MoveGen& moves)
{
    bool isLegalMove;
    std::vector<Move> legal_move_list;

    for (auto move : moves.move_list())
    {
        isLegalMove = moves.make_move(move);
        if (!isLegalMove) continue;
        
        legal_move_list.push_back(move);
        moves.undo_move();
    }
    return legal_move_list;
}

int quiesce(Position& pos, MoveGen& moves, int alpha, int beta)
{
    /*
    The quiescence search makes sure to only stop the search in quiet 
    positions (no captures or checks) to avoid the horizon effect.
    */
    int stand_pat = Eval::evaluate(pos);
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;
    int score;
    auto move_list = filter_illegal_moves(moves);

    for (Move move : move_list)
    {
        if (!move.has_capture_flag()) break;
        moves.make_move(move);
        score = -quiesce(pos, moves, -beta, -alpha);
        moves.undo_move();

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }

    return alpha;

}

int alpha_beta(Position& pos, MoveGen& moves, int alpha, int beta, int depthLeft)
{
    if (depthLeft <= 0) return quiesce(pos, moves, alpha, beta);
    moves.generate_all_moves();
    auto moveList = filter_illegal_moves(moves);
    int score;
    for (Move move : moveList)
    {
        score = -alpha_beta(pos, moves, -beta, -alpha, depthLeft - 1);
        if (score >= beta) return beta;
        if (score > alpha) alpha = score; 
    }

    return alpha;
}

RootMove search(Position& pos, int depthLeft)
{
    const Move nullMove = Move(Square::A1, Square::A1, Flag::QUIET);
    const int infinity = std::numeric_limits<int>::max();
    int alpha = -infinity; 
    int beta = infinity;
    Move bestMove = nullMove;
    MoveGen moves(pos);
    if (depthLeft == 0)
    {
        int eval = quiesce(pos, moves, alpha, beta);
        return {nullMove, eval};
    }

    moves.generate_all_moves();
    auto moveList = filter_illegal_moves(moves);
    int score;
    for (Move move : moveList)
    {
        score = -alpha_beta(pos, moves, -beta, -alpha, depthLeft);
        if (score >= beta) 
        {
            bestMove = move;
            //return {bestMove, beta};
        }
        if (score > alpha)
        {
            alpha = score;
            bestMove = move;
        }
    }

    return {bestMove, alpha};

}









}