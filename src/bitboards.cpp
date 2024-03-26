#include "bitboards.hpp"
namespace Kite::Bitboard {

int population_count(const U64 bitboard) {
    int count = 0;
    auto bb = bitboard;
    while (bb) {
        count += bb & 0x1ULL;
        bb >>= 1;
    }
    return count;
}

Square msb(const U64 bitboard) 
{
    // most significant bit in bitboard
    U64 mask = set_bit(Square::H8);

    for (int sq = 63; sq >= 0; sq--, mask >>=1)
    {
        if ((bitboard & mask) == mask) {
            return static_cast<Square>(sq);
        }
    }

    return Square::EMPTY_SQUARE;
}

Square lsb(const U64 bitboard) 
{
    Square sq;
    U64 sqbb;
    for (int i = 0; i < 64; i++) {
        sq = static_cast<Square>(i);
        sqbb = set_bit(sq);
        if ((bitboard & sqbb) == sqbb) {
            return sq;
        }
    }

    return Square::EMPTY_SQUARE;
}

Square pop_lsb(U64& bitboard) {
    Square sq = lsb(bitboard);
    bitboard = bitboard - (bitboard & -bitboard);
    return sq;
}

std::vector<Square> get_squares(const U64 bb)
{
    U64 bbCopy = bb;
    std::vector<Square> result;
    
    while (!is_empty(bbCopy))
    {
        Square sq = pop_lsb(bbCopy);
        result.push_back(sq);
    }
    return result;
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
    const U64 left = (kingPosition & ~file::A) >> 1;
    const U64 right = (kingPosition & ~file::H) << 1;
    U64 attacks = left | right;

    kingPosition |= attacks;

    const U64 up = (kingPosition & ~rank::eighth) << 8;
    const U64 down = (kingPosition & ~rank::first) >> 8;

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

U64 pawn_attacks(const U64 bb, const Color color) {
    const U64 notA = ~file::A; 
    const U64 notH = ~file::H;
    const U64 leftBB = (color == Color::WHITE) ? (bb & notA) : (bb & notH);
    const U64 rightBB = (color == Color::WHITE) ? (bb & notH) : (bb & notA);

    const U64 left = (color == Color::WHITE) ? (leftBB) << 7 : leftBB >> 7;
    const U64 right = (color == Color::WHITE) ? rightBB << 9 : rightBB >> 9;

    return left | right;
}


U64 pawn_push(const U64 bb, const Color color)
{
    U64 push = (color == Color::WHITE) ? (bb << 8) : (bb >> 8);
    return push;
}
U64 double_pawn_push(const U64 bb, Color color)
{
    U64 mask = (color == Color::WHITE) ? 0xff00ULL : 0xff000000000000ULL;
    U64 push = (color == Color::WHITE) ? ((mask & bb) << 16) : ((mask & bb) >> 16);
    return push;
}

/*
    Using classical approach instead of magic bitboards for sliding pieces
*/

U64 rook_attacks(Square sq, const U64 blockers) {
    U64 attacks = 0x0ULL;
    attacks |= Rays::get_ray_attacks(sq, Rays::Direction::NORTH);

    if (Rays::get_ray_attacks(sq, Rays::Direction::NORTH)) {
        Square blockerIndex = (lsb(Rays::get_ray_attacks(sq, Rays::Direction::NORTH) & blockers));
        attacks &= ~Rays::get_ray_attacks(blockerIndex, Rays::Direction::NORTH);
    }
    attacks |= Rays::get_ray_attacks(sq, Rays::Direction::SOUTH);

    if (Rays::get_ray_attacks(sq, Rays::Direction::SOUTH)) {
        Square blockerIndex = (msb(Rays::get_ray_attacks(sq, Rays::Direction::SOUTH) & blockers));
        attacks &= ~Rays::get_ray_attacks(blockerIndex, Rays::Direction::SOUTH);
    }
    attacks |= Rays::get_ray_attacks(sq, Rays::EAST);
    if (Rays::get_ray_attacks(sq, Rays::EAST)) {
        Square blockerIndex = (lsb(Rays::get_ray_attacks(sq, Rays::EAST) & blockers));
        attacks &= ~Rays::get_ray_attacks(blockerIndex,Rays::EAST);
    }
    attacks |= Rays::get_ray_attacks(sq, Rays::WEST);
    if (Rays::get_ray_attacks(sq, Rays::WEST)) {
        Square blockerIndex = (msb(Rays::get_ray_attacks(sq, Rays::WEST) & blockers));
        attacks &= ~Rays::get_ray_attacks(blockerIndex, Rays::WEST);
    }
    return attacks;
    

}

U64 bishop_attacks(Square sq, const U64 blockers) {
    U64 attacks = 0x0ULL;
    attacks |= Rays::get_ray_attacks(sq, Rays::NE);

    if (Rays::get_ray_attacks(sq, Rays::NE)) {
        Square blockerIndex = (lsb(Rays::get_ray_attacks(sq, Rays::NE) & blockers));
        attacks &= ~Rays::get_ray_attacks(blockerIndex, Rays::NE);    
    }
    attacks |= Rays::get_ray_attacks(sq, Rays::SE);

    if (Rays::get_ray_attacks(sq, Rays::SE)) {
        Square blockerIndex = (msb(Rays::get_ray_attacks(sq, Rays::SE) & blockers));
        attacks &= ~Rays::get_ray_attacks(blockerIndex, Rays::SE);
    }
    attacks |= Rays::get_ray_attacks(sq,Rays::NW);
    if (Rays::get_ray_attacks(sq, Rays::NW)) {
        Square blockerIndex = (lsb(Rays::get_ray_attacks(sq, Rays::NW) & blockers));
        attacks &= ~Rays::get_ray_attacks(blockerIndex, Rays::NW);
    }
    attacks |= Rays::get_ray_attacks(sq, Rays::SW);
    if (Rays::get_ray_attacks(sq, Rays::SW)) {
        Square blockerIndex = (msb(Rays::get_ray_attacks(sq, Rays::SW) & blockers));
        attacks &= ~Rays::get_ray_attacks(blockerIndex, Rays::SW);
    }
    return attacks;
}

}
