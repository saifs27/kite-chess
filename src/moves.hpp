#pragma once
#include "types.hpp"
namespace Smyslov {



struct Move
{
    private:
    Square _from;
    Square _to;
    Flag _flag;

    public:
    Move(Square from, Square to, Flag flag)
    : _from(from), _to(to), _flag(flag)
    {}

    const Square from() const {return _from;} ;
    const Square to() const {return _to;};
    const Flag flags() const {return _flag;};

    void set_from(Square from){_from = from;};
    void set_to(Square to) {_to = to;};
    void set_flags(Flag flags) {_flag = flags;};

    bool has_capture_flag() const;
    bool is_capture(const U64 opponentBB) const;
    Piece get_promotion_piece() const;
    bool switch_flag_to_capture();
    Square get_enPassant_square() const;


    

};

/*

More memory efficient but harder to debug. Will switch to this later.

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
*/


}