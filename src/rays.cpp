#include "rays.hpp"

namespace Smyslov {

void Rays::init() {
    U64 north = 0x0101010101010100ULL;
    U64 south = 0x80808080808080;
    U64 west = 0x7f00000000000000;
    U64 east = 0xfe;
    U64 ne = 0x8040201008040200ULL;
    U64 nw = 0x102040810204000ULL;
    U64 sw = 0x40201008040201;
    U64 se = 0x2040810204080;
    for (int sq = 0; sq < 64; sq++, north <<=1, south >>=1) {
        rayAttacks[sq][static_cast<int>(NORTH)] = north;
        rayAttacks[63-sq][static_cast<int>(SOUTH)] = south;  
    }


    for (int sq = 0; sq < 64; sq++, east <<=1, west >>=1) {
        rayAttacks[sq][static_cast<int>(EAST)] = east & get_rank_mask<int>(sq);
        rayAttacks[63-sq][static_cast<int>(WEST)] = west & get_rank_mask<int>(63-sq);
    }

    for (int f = 0; f < 8; f++, ne <<= 1, nw >>=1) {
        U64 northeast = ne;
        U64 northwest = nw;
        for (int r = 0; r < 64; r += 8, northeast <<= 8, northwest <<= 8) {
            rayAttacks[r+f][static_cast<int>(NE)] = northeast & mask_east(r+f);
            rayAttacks[r+(7-f)][static_cast<int>(NW)] = northwest & mask_west(r+(7-f));
        } 
    }

    for (int f = 0; f < 8; f++, se <<= 1, sw >>=1) {
        U64 southeast = se;
        U64 southwest = sw;
        for (int r = 56; r >= 0; r -= 8, southeast >>=8, southwest >>=8) {
            rayAttacks[r+f][static_cast<int>(SE)] = southeast & mask_east(r+f);
            rayAttacks[r+(7-f)][static_cast<int>(SW)] = southwest & mask_west(r+(7-f));
        } 
    }

}


}

