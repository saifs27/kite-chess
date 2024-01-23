#include "bitboards.hpp"
namespace Smyslov {

bool is_empty(U64 bb)  {
    return bb == 0x0ULL;
}

bool has(U64 bb, Square square) {
    U64 check_sq = bb & set_bit(square);
    return !(is_empty(check_sq));
}


int population_count(const U64 bitboard) {
    int count = 0;
    auto bb = bitboard;
    while (bb) {
        count += bb & 0x1ULL;
        bb >>= 1;
    }
    return count;
}

Square msb(const U64 bitboard) {
    U64 mask = set_bit(H8);

    for (int sq = 63; sq >= 0; sq--, mask >>=1)
    {

        if ((bitboard & mask) == mask) {
            return static_cast<Square>(sq);
        }
    }

    return EMPTY_SQUARE;
}

Square lsb(const U64 bitboard) {
    Square sq;
    U64 sqbb;
    for (int i = 0; i < 64; i++) {
        sq = static_cast<Square>(i);
        sqbb = set_bit(sq);
        if ((bitboard & sqbb) == sqbb) {
            return sq;
        }
    }
    return EMPTY_SQUARE;
}

Square pop_lsb(U64& bitboard) {
    Square sq = lsb(bitboard);
    bitboard = bitboard - (bitboard & -bitboard);
    return sq;
}

void print_bitboard(const U64 bitboard) {
    U64 last_sq = 0x8000000000000000ULL;
    std::string printed_board[64];
    for (int i = 63; i >= 0; i--) {
        if ((bitboard & last_sq) != 0) {
            printed_board[i] = " 1";
        }
        else {
            printed_board[i] = " 0";
        }
        last_sq >>= 1;
    }

        for (int rank = 7; rank >= 0; rank--) 
    {
        for (int file = 0; file < 8; file++) 
        {
            int sq = rank * 8 + file;
            if (sq % 8 == 0) { std::cout << '\n';}
            std::cout << printed_board[sq];
        }

    }
    std::cout << '\n';
}

U64 king_attacks(const U64 bb) {
    U64 kingPosition = bb;
    const U64 left = kingPosition >> 1;
    const U64 right = kingPosition << 1;
    U64 attacks = left | right;

    kingPosition |= attacks;

    const U64 up = kingPosition << 8;
    const U64 down = kingPosition >> 8;

    attacks |= up | down;

    return attacks;
}

U64 knight_attacks(const U64 bb) {
    const U64 notAB = ~file::A & ~file::B;
    const U64 notGH = ~file::G & ~file::H;
    const U64 notA = ~file::A;
    const U64 notH = ~file::H;

    const U64 a = (bb & notH) << 17;
    const U64 b = (bb & notA) << 15;
    const U64 c = (bb & notGH) << 10;
    const U64 d = (bb & notAB) << 6;

    const U64 e = (bb & notA) >> 17;
    const U64 f = (bb & notH) >> 15;
    const U64 g = (bb & notAB) >> 10;
    const U64 h = (bb & notGH) >> 6;

    return a | b | c | d | e | f | g | h ;
}

U64 pawn_attacks(const U64 bb) {
    const U64 notA = ~file::A; 
    const U64 notH = ~file::H;


    const U64 left = (bb & notA) << 7;
    const U64 right = (bb & notH) << 9;

    return left | right;
}


U64 pawn_push(Color color, const U64 bb)
{
    U64 push = (color == WHITE) ? (bb << 8) : (bb >> 8);
    return push;
}
U64 double_pawn_push(Color color, const U64 bb)
{
    U64 mask = (color == WHITE) ? 0xff00ULL : 0xff000000000000ULL;
    U64 push = (color == WHITE) ? ((mask & bb) << 16) : ((mask & bb) >> 16);
    return push;
}








/*
    Using classical approach instead of magic bitboards for sliding pieces
*/

U64 rook_attacks(Square sq, const U64 blockers) {
    U64 attacks = 0x0ULL;
    attacks |= Rays::rayAttacks[sq][Rays::NORTH];

    if (Rays::rayAttacks[sq][Rays::NORTH]) {
        int blockerIndex = lsb(Rays::rayAttacks[sq][Rays::NORTH] & blockers);
        attacks &= ~Rays::rayAttacks[blockerIndex][Rays::NORTH];
    }
    attacks |= Rays::rayAttacks[sq][Rays::SOUTH];

    if (Rays::rayAttacks[sq][Rays::SOUTH]) {
        int blockerIndex = msb(Rays::rayAttacks[sq][Rays::SOUTH] & blockers);
        attacks &= ~Rays::rayAttacks[blockerIndex][Rays::SOUTH];
    }
    attacks |= Rays::rayAttacks[sq][Rays::EAST];
    if (Rays::rayAttacks[sq][Rays::EAST]) {
        int blockerIndex = lsb(Rays::rayAttacks[sq][Rays::EAST] & blockers);
        attacks &= ~Rays::rayAttacks[blockerIndex][Rays::EAST];
    }
    attacks |= Rays::rayAttacks[sq][Rays::WEST];
    if (Rays::rayAttacks[sq][Rays::WEST]) {
        int blockerIndex = msb(Rays::rayAttacks[sq][Rays::WEST] & blockers);
        attacks &= ~Rays::rayAttacks[blockerIndex][Rays::WEST];
    }
    return attacks;
    

}

U64 bishop_attacks(Square sq, const U64 blockers) {
    U64 attacks = 0x0ULL;
    attacks |= Rays::rayAttacks[sq][Rays::NE];

    if (Rays::rayAttacks[sq][Rays::NE]) {
        int blockerIndex = lsb(Rays::rayAttacks[sq][Rays::NE] & blockers);
        attacks &= ~Rays::rayAttacks[blockerIndex][Rays::NE];
    }
    attacks |= Rays::rayAttacks[sq][Rays::SE];

    if (Rays::rayAttacks[sq][Rays::SE]) {
        int blockerIndex = msb(Rays::rayAttacks[sq][Rays::SE] & blockers);
        attacks &= ~Rays::rayAttacks[blockerIndex][Rays::SE];
    }
    attacks |= Rays::rayAttacks[sq][Rays::NW];
    if (Rays::rayAttacks[sq][Rays::NW]) {
        int blockerIndex = lsb(Rays::rayAttacks[sq][Rays::NW] & blockers);
        attacks &= ~Rays::rayAttacks[blockerIndex][Rays::NW];
    }
    attacks |= Rays::rayAttacks[sq][Rays::SW];
    if (Rays::rayAttacks[sq][Rays::SW]) {
        int blockerIndex = msb(Rays::rayAttacks[sq][Rays::SW] & blockers);
        attacks &= ~Rays::rayAttacks[blockerIndex][Rays::SW];
    }
    return attacks;
}




}
