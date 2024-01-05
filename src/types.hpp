#pragma once
#include <optional>

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
enum Rank {First, Second, Third, Fourth, Fifth};

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

enum Castling {WhiteKingside = 1, WhiteQueenside = 2, BlackKingside = 4, BlackQueenside = 8};

/*
White kingside castling: 0001
0010
0100
1000
*/


enum Color {WHITE, BLACK, NONE}; 
enum Piece {PAWN = 2, KNIGHT, BISHOP, ROOK, QUEEN, KING, EMPTY}; // to access position bitboard array
enum MoveType {NORMAL, CASTLING, ENPASSANT, PROMOTION};

struct Move {
    MoveType moveType;
    Square from;
    Square to;
    Piece piece;
    Color color;
    Piece promoted;
};


U64 set_bit(Square sq) {
    return 0x1ULL << sq;
}

File get_file(Square square){
    int file = square / 8;
    return static_cast<File>(file);
}
Rank get_rank(Square square){
    int rank = square % 8;
    return static_cast<Rank>(rank);
}

std::optional<Square> try_offset(Square sq, int file_offset, int rank_offset){
    File file = get_file(sq);
    Rank rank = get_rank(sq);
    int new_file = file + file_offset;
    int new_rank = rank + rank_offset;
    if (new_file >= 8 || new_rank >= 8) {
        return {};
    }

    return static_cast<Square>(new_rank * 8 + new_file);
}
