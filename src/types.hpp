#pragma once
#include <optional>
#include <iostream>

namespace Smyslov {
typedef unsigned long long U64;

enum Square {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    EMPTY_SQUARE
};

enum File {A, B, C, D, E, F, G, H};
enum Rank {First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth};

namespace file {
inline constexpr U64 A = 0x101010101010101ULL;
inline constexpr U64 B = 0x202020202020202ULL;
inline constexpr U64 C = 0x404040404040404ULL;
inline constexpr U64 D = 0x808080808080808ULL;
inline constexpr U64 E = 0x1010101010101010ULL;
inline constexpr U64 F = 0x2020202020202020ULL;
inline constexpr U64 G = 0x4040404040404040ULL;
inline constexpr U64 H = 0x8080808080808080ULL;
}

namespace rank {
inline constexpr U64 first   = 0xff;
inline constexpr U64 second  = 0xff00;
inline constexpr U64 third   = 0xff0000;
inline constexpr U64 fourth  = 0xff000000;
inline constexpr U64 fifth   = 0xff00000000;
inline constexpr U64 sixth   = 0xff0000000000;
inline constexpr U64 seventh = 0xff000000000000;
inline constexpr U64 eighth  = 0xff00000000000000;
}

enum Castling {WhiteKingside = 1, WhiteQueenside = 2, BlackKingside = 4, BlackQueenside = 8};

/*
White kingside castling: 0001
0010
0100
1000
*/


enum Color {WHITE, BLACK, NONE}; 
enum Piece {PAWN = 2, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY}; // starts at 2 to access position bitboard array

enum Flag {
    NOFLAG,
    EN_PASSANT,
    CASTLE,
    CAPTURE,
    DOUBLE_PAWN,
    PROMOTE_QUEEN,
    PROMOTE_KNIGHT,
    PROMOTE_ROOK,
    PROMOTE_BISHOP
};

struct Move
{
    private:
    short moveValue;

    public:
    Move(Square from, Square to, Flag flag)
    {
        moveValue =  from | (to << 6) | (flag << 12);
    }

    Square from() const {
        short mask = 0b0000000000111111;
        int sq = mask & moveValue;
        return static_cast<Square>(sq);
    }
    Square to() const {
        short mask = 0b0000111111000000;
        int sq = (mask & moveValue) >> 6;
        return static_cast<Square>(sq);
    }

    Flag flags() const {
        short mask  = 0b1111000000000000;
        return static_cast<Flag>((moveValue & mask) >> 12);
    }

    void set_from(Square from) {
        short mask  = 0b0000000000111111;
        moveValue &= ~mask;
        moveValue |= from;
    }

    void set_to(Square to) {
        short mask  = 0b0000111111000000;
        moveValue &= ~mask;
        moveValue |= (to << 6);
    }

    void set_flags(Flag flags) {
        short mask  = 0b1111000000000000;
        moveValue &= ~mask;
        moveValue |= (flags << 12);
    }


};



inline U64 set_bit(Square sq) {
    return 0x1ULL << sq;
}

inline File get_file(Square square){
    int file = square % 8;
    return static_cast<File>(file);
}
inline Rank get_rank(Square square){
    int rank = square / 8;
    return static_cast<Rank>(rank);
}

template <typename T>
inline U64 get_file_mask(T square){
    int file = square % 8;
    switch (file) {
        case 0:
        return file::A;
        case 1:
        return file::B;
        case 2:
        return file::C;
        case 3:
        return file::D;
        case 4:
        return file::E;
        case 5:
        return file::F;
        case 6:
        return file::G;
        case 7:
        return file::H;
        default:
        return 0x0ULL;
    }
}

template <typename T>
inline U64 get_rank_mask(const T square){
    const int rank = square / 8;
    switch (rank) {
        case 0:
        return rank::first;
        case 1:
        return rank::second;
        case 2:
        return rank::third;
        case 3:
        return rank::fourth;
        case 4:
        return rank::fifth;
        case 5:
        return rank::sixth;
        case 6:
        return rank::seventh;
        case 7:
        return rank::eighth;
        default:
        return 0x0ULL;
    }
}

template <typename T>
inline U64 mask_west(const T sq)
{
    U64 mask = 0x0ULL;

    const U64 stop = get_file_mask(sq);

    for (U64 file = file::A; file < stop; file <<= 1)
    {

        mask |= file;
    }

    return mask;

}

template <typename T>
inline U64 mask_east(const T sq)
{
    U64 mask = 0x0ULL;
    const U64 stop = get_file_mask(sq);

    for (U64 file = file::H; file > stop; file >>=1)
    {
        mask |= file;
    }

    return mask;
}

inline std::optional<Square> try_offset(Square sq, int file_offset, int rank_offset){
    int file = get_file(sq) + file_offset;
    int rank = get_rank(sq) + rank_offset;
    int new_square = rank * 8 + file;
    if (file >= 8 || file < 0 || rank >= 8 || rank < 0) 
    {
        return {};
    }

    return static_cast<Square>(new_square);
}
}


