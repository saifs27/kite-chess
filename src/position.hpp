#include <vector>
#include "types.hpp"
#include "bitboards.hpp"

class Position { 
    private:
    std::vector<Move> moveHistory; 
    public:
    U64 pieceBitBoard[8];
    ColorType side;
    int enPassant;
    int castlingPerm;



    Position();
    void startPosition();
    U64 getPieceBB(const ColorType color, const PieceType piece);
    U64 attacks(const ColorType color);
    bool can_castle(const Castling castleType);
    bool pseudo_legal(const Move move);
    bool legal(const Move move);
    bool make_move(const Move move);
    void unmake_move();
};