#include "board.hpp"

class Move {
    public:
    void quiet(const BitBoard position, int move);
    void capture(const BitBoard position, int move);
    void en_passant(const BitBoard position, int move);
    void quiet_move(const BitBoard position, int move);
    void white_pawn_capture(const BitBoard position, int move);
    void white_pawn(const BitBoard position, int move);
    void all_moves(const BitBoard position, int move);
};