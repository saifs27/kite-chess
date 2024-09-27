#include "zobrist.hpp"

namespace Kite {

int PositionKey::get_piece_key(Square square, Color color, Piece piece) const {
    int color_shift = color == Color::WHITE ? 0 : 6;
    int index = static_cast<int>(square) * 12 + static_cast<int>(piece) + color_shift;
    return piece_keys.at(index);
}

int PositionKey::get_castling_key(short castling) const {
    short ptr = 0b0001;
    U64 result = 0;
    for (int i = 0; i < 4; i++) {
        if ((castling & ptr) != 0) result ^= castling_rights[i];
        ptr <<= 1;
    }
    return result;
}

int PositionKey::get_en_pas_key(File en_pas) const {
    if (en_pas == File::NO_FILE) return 0;
    return en_pas_file[static_cast<int>(en_pas)];
};


void PositionKey::init() { 
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1, 0xfffffff);


    for (int i = 0, len = piece_keys.size(); i < len; i++) {
        piece_keys[i] = dist(rd);
    } 

    for (int j = 0, len = side.size(); j < len; j++) {
        side[j] = dist(rd);
    }

    for (int k = 0, len = castling_rights.size(); k < len; k++) {
        castling_rights[k] = dist(rd);
    }

    for (int l = 0, len = en_pas_file.size(); l < len; l++) {
        en_pas_file[l] = dist(rd);
    }

}



} // namespace Kite::Zobrist