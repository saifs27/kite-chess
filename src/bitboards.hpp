#pragma once
#include "types.hpp"
#include "position.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <random>
#include <optional>
namespace Smyslov {
class Bitboard {
    public:
    U64 bitboard;

    Bitboard(U64 bb) : bitboard(bb) {};
    // operator overloading
    Bitboard operator|(Bitboard bb) const {return Bitboard {bitboard | bb.bitboard};}
    void operator|=(Bitboard bb){bitboard |= bb.bitboard;}
    Bitboard operator&(Bitboard bb) const {return Bitboard {bitboard & bb.bitboard};}
    void operator&=(Bitboard bb){bitboard &= bb.bitboard;}
    Bitboard operator^(Bitboard bb) const {return Bitboard {bitboard ^ bb.bitboard};}
    void operator^=(Bitboard bb){bitboard ^= bb.bitboard;}
    Bitboard operator~() const {return Bitboard {~bitboard};}
    Bitboard operator<<(int n) const {return Bitboard {bitboard << n};}
    void operator<<=(int n){bitboard <<= n;}
    Bitboard operator>>(int n) const {return Bitboard {bitboard >> n};}
    void operator>>=(int n){bitboard >>= n;}
    void operator=(Bitboard bb){this->bitboard = bb.bitboard;}
    bool operator==(Bitboard bb){return bitboard == bb.bitboard;}
    bool operator!=(Bitboard bb){return bitboard != bb.bitboard;}

    bool is_empty() const;
    bool has(Square sq) const;
    std::vector<Bitboard> get_subsets();
    

    int population_count() const;
    Square msb() const;
    Square lsb() const;
    Square pop_lsb();
    void print_bitboard() const;

};




U64 king_attacks(const U64 bb);
U64 knight_attacks(const U64 bb);
U64 pawn_attacks(const U64 bb);
U64 bishop_attacks(const U64 bb);
U64 rook_attacks(const U64 bb);



}




