#include "magic.hpp"

Bitboard Slider::moves(const Square square, Bitboard blockers){
    Bitboard moves {0x0ULL};

    for (auto i: deltas) {
        Square ray = square;
        while (!blockers.has(ray)) {
            std::optional<Square>(shifted) = ray;
            if (shifted.has_value()) {
                ray = shifted.value();
                moves |= Bitboard{set_bit(ray)};
            }
            else {break;}
        }
    }
    return moves;
}

Bitboard Slider::get_relevant_blockers(const Square sq){
    Bitboard blockers {0x0ULL};
    for (auto i: deltas) {
        Square ray = sq;
        std::optional<Square>(shifted) = ray;
        while (shifted.has_value()) {
            blockers |= Bitboard {set_bit(ray)};
            ray = shifted.value();
        }

    }
    blockers &= ~(Bitboard {set_bit(sq)});
    return blockers;
}

U64 magic_index(const Magic& entry, const U64 blockers) {
    /*
        Magic Hashing function:

        (blockers * magic) >> (64 - n)
        where blockers = blockers & mask

        We will shift by n instead
        and store 64-n to remove
        one subtraction at runtime.

    */

    U64 blocker_mask = blockers & entry.mask;
    U64 hash = blocker_mask % (0x1ULL << entry.magic);
    U64 index = (hash >> entry.shift);
    return entry.offset + index;
}


void find_magic(Slider& slider, const Square square, const u_int8_t index_bits) {
    Bitboard mask = slider.get_relevant_blockers(square);

    while (true) {
        std::uniform_int_distribution<U64> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2,62)));
        
    }

    
}

U64 get_rook_moves(Square sq, U64 blockers){
    //Magic magic = ROOK_MAGICS[sq];
    //return ROOK_MOVES[magic_index(magic, blockers)];
}

U64 get_bishop_moves(Square sq, U64 blockers){
    //Magic magic = BISHOP_MAGICS[sq];
    //return BISHOP_MOVES[magic_index(magic, blockers)];
}