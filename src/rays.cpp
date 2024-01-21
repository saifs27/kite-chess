#include "rays.hpp"

namespace Smyslov {

void Rays::init() {
    U64 north = 0x0101010101010100ULL;
    U64 south = 0x80808080808080;
    U64 west = 0x7f00000000000000;
    U64 east = 0xfe;
    U64 ne = 0x8040201008040200;
    U64 nw = 0x102040810204000;
    U64 sw = 0x40201008040201;
    U64 se = 0x2040810204080;
    for (int sq = 0; sq < 64; sq++, north <<=1, south >>=1) {
        rayAttacks[sq][NORTH] = north;
        rayAttacks[63-sq][SOUTH] = south;
    }


    for (int sq = 0; sq < 64; sq++, east <<=1, west >>=1) {
        rayAttacks[sq][EAST] = east & get_rank_mask<int>(sq);
        rayAttacks[63-sq][WEST] = west & get_rank_mask<int>(63-sq);
    }

    for (int f = 0; f < 8; f++, ne += 9, se-=7) {
        for (int r = 0; r < 64; r += 8, ne <<= 8, se<<=8) {
            rayAttacks[r+f][NE] = ne;
            rayAttacks[r+f][SE] = se;
        } 
    }

    for (int f = 0; f < 8; f++, nw += 7, sw-=9) {
        for (int r = 0; r < 64; r += 8, nw <<= 8, sw<<=8) {
            rayAttacks[r+f][NW] = nw;
            rayAttacks[r+f][SW] = sw;
        } 
    }

}


}

