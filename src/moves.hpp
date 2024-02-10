#pragma once
#include "types.hpp"
namespace Smyslov {

struct Move
{
    private:
    short moveValue;

    public:
    Move(Square from, Square to, Flag flag)
    {
        int start = static_cast<int>(from);
        int end = static_cast<int>(to);
        int flags = static_cast<int>(flag);

        moveValue =  start | (end << 6) | (flags << 12);
    }

    const Square from() const;
    const Square to() const;
    const Flag flags() const;

    void set_from(Square from);
    void set_to(Square to);
    void set_flags(Flag flags);

    bool has_capture_flag() const;
    bool is_capture(const U64 opponentBB) const;
    Piece get_promotion_piece() const;
    bool switch_flag_to_capture();
    Square get_enPassant_square() const;

    

};







}