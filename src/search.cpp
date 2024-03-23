#include "search.hpp"

namespace Smyslov::Search {

void order_moves(const Position& pos, std::vector<Move> moves)
{
    for (Move move : moves)
    {
        int moveScore = 0;
        Piece fromPieceType = pos.get_piece(move.from());

        if (move.has_capture_flag())
        {
            Piece capturePieceType = pos.get_piece(move.to());
            moveScore = 10 * get_piece_value(capturePieceType) - get_piece_value(fromPieceType);
                       
        }

    }
}

std::vector<Move> filter_illegal_moves(MoveGen& moves)
{
    moves.generate_all_moves();

    bool isLegalMove;
    std::vector<Move> legal_move_list;
    for (auto move : moves.move_list())
    {
        isLegalMove = moves.make_move(move);
        if (isLegalMove)
        {
            legal_move_list.push_back(move);
            moves.undo_move();
        }
    }
    return legal_move_list;
}

int quiesce(Position& pos, MoveGen& moves, int alpha, int beta)
{
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

int search(Position& pos, int depth)
{
    if (depth == 0) return Eval::evaluate(pos);

    int max = -std::numeric_limits<int>::max();
    MoveGen moves(pos);

    moves.generate_all_moves();
    auto legal_move_list = filter_illegal_moves(moves);

    if (legal_move_list.empty())
    {
        if (pos.is_check()) return -std::numeric_limits<int>::max();
        return 0;
    }

    for (Move move : legal_move_list)
    {
        moves.make_move(move);
        int score = -search(pos, depth -1);
        if (score > max) max = score;
        moves.undo_move();        
    }
    return max;

}












}