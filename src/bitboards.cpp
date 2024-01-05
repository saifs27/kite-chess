#include "bitboards.hpp"
template <typename T>
Bitboard Bitboard::operator|(T bb){
    return Bitboard {bitboard | bb.bitboard};
}

void Bitboard::operator|=(Bitboard bb){
     bitboard |= bb.bitboard;
}

Bitboard Bitboard::operator&(Bitboard bb){
    return Bitboard {bitboard & bb.bitboard};
}

void Bitboard::operator&=(Bitboard bb){
     bitboard &= bb.bitboard;
}

Bitboard Bitboard::operator^(Bitboard bb){
    return Bitboard {bitboard ^ bb.bitboard};
}

void Bitboard::operator^=(Bitboard bb){
     bitboard ^= bb.bitboard;
}

Bitboard Bitboard::operator~(){
    return Bitboard {~bitboard};
}

Bitboard Bitboard::operator<<(int n){
    return Bitboard {bitboard << n};
}

void Bitboard::operator<<=(int n){
     bitboard <<= n;
}

Bitboard Bitboard::operator>>(int n){
    return Bitboard {bitboard >> n};
}

void Bitboard::operator>>=(int n){
     bitboard >>= n;
}
template <typename T>
void Bitboard::operator=(T bitboard){
     this->bitboard = bitboard;
}
template <typename T>
bool Bitboard::operator==(T bb){
    return bitboard == bb.bitboard;
}
template <typename T>
bool Bitboard::operator!=(T bb){
     return bitboard != bb.bitboard;
}




bool Bitboard::is_empty() const {
    return bitboard == 0x0ULL;
}

bool Bitboard::has(Square square) const {
    Bitboard check_sq {(bitboard & set_bit(square))};
    return !(check_sq.is_empty());
}




int Bitboard::population_count() const {
    int count = 0;
    U64 bb = bitboard;
    while (bb) {
        count += bb & 0x1ULL;
        bb >>= 1;
    }
    return count;
}

Square Bitboard::lsb() const {
    U64 bit = 0x1ULL;
    for (int i = 0; i < 64; i++) {
        Square sq = static_cast<Square>(i);
        if (bitboard & (set_bit(sq)) != 0) {
            return sq;
        }
    }
    return EMPTY_SQUARE;
}

Square Bitboard::pop_lsb() {
    Square sq = lsb();
    bitboard = bitboard - (bitboard & -bitboard);
    return sq;
}

void Bitboard::print_bitboard() const {
    U64 last_sq = 0x8000000000000000ULL;
    U64 bb = bitboard;
    std::string printed_board[64];
    for (int i = 0; i < 64; i++) {
        if ((bitboard & last_sq) != 0) {
            printed_board[i] = " 1";
        }
        else {
            printed_board[i] = " 0";
        }
        last_sq >>= 1;
    }

    for (int sq = 0; sq < 64; sq++) {
        if (sq % 8 == 0) { std::cout << '\n';}
        std::cout << printed_board[sq];
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

U64 rook_attacks(const U64 bb) {
    //TODO
    return bb;
    

}

U64 bishop_attacks(const U64 bb) {
    //TODO
    return bb;
}




