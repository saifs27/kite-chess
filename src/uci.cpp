#include "uci.hpp"

namespace Smyslov{
namespace UCI {
std::string square_to_string(Square sq)
{
    File sqFile = get_file(sq);
    Rank sqRank = get_rank(sq);
    std::string square;
    switch (sqFile)
    {
        case File::A :
        square.append("a"); break;
        case File::B:
        square.append("b"); break;
        case File::C :
        square.append("c"); break;
        case File::D:
        square.append("d"); break;
        case File::E:
        square.append("e"); break;
        case File::F:
        square.append("f"); break;
        case File::G:
        square.append("g"); break;
        case File::H:
        square.append("h"); break;
    }
    switch (sqRank)
    {
        case Rank::First :
        square.append("1"); break;
        case Rank::Second:
        square.append("2"); break;
        case Rank::Third :
        square.append("3"); break;
        case Rank::Fourth:
        square.append("4"); break;
        case Rank::Fifth:
        square.append("5"); break;
        case Rank::Sixth:
        square.append("6"); break;
        case Rank::Seventh:
        square.append("7"); break;
        case Rank::Eighth:
        square.append("8"); break;
    }
    return square;
}

std::string piece_to_string(Piece piece)
{
    switch (piece)
    {
        case Piece::ROOK:
        return "R";
        case Piece::KNIGHT:
        return "N";
        case Piece::BISHOP:
        return "B";
        case Piece::QUEEN:
        return "Q";
        case Piece::KING:
        return "K";
        default:
        return "";
    }
}

std::string move_to_uci(Move move)
{
    std::string uciResult;
    uciResult.append(square_to_string(move.from()));
    uciResult.append(square_to_string(move.to()));
    Piece promotedPiece = move.get_promotion_piece();
    uciResult.append(piece_to_string(promotedPiece));
    return uciResult;
}

    
}
}