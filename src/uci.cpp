/*
    Kite, a UCI compliant chess engine.
    Copyright (C) 2024  Saif

    Kite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Kite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/


#include "uci.hpp"

namespace Kite::UCI{

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

Piece string_to_piece(const char string)
{
    if (string == 'Q') return Piece::QUEEN;
    else if (string == 'R') return Piece::ROOK;
    else if (string == 'N' ) return Piece::KNIGHT;
    else if (string == 'B') return Piece::BISHOP;
    else return Piece::EMPTY;
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

std::optional<Move> uci_to_move(Position& pos, std::string uci)
{
    Move move(Square::A1, Square::A1, Flag::QUIET);

    if (uci.compare("0000") == 0) {return move;}

    int from_file = static_cast<int>(uci[0])-97;
    int from_rank = static_cast<int>(uci[1])-49;
    int to_file = static_cast<int>(uci[2])-97;
    int to_rank = static_cast<int>(uci[3])-49;

    Piece promoted = Piece::EMPTY;

    if (uci.size() == 5)
    {
        Flag promoteFlag;
        promoted = string_to_piece(uci.at(4));
        switch(promoted)
        {
            case Piece::QUEEN: promoteFlag = Flag::PROMOTE_QUEEN; break;
            case Piece::ROOK: promoteFlag = Flag::PROMOTE_ROOK; break;
            case Piece::KNIGHT: promoteFlag = Flag::PROMOTE_KNIGHT; break;
            case Piece::BISHOP: promoteFlag = Flag::PROMOTE_BISHOP; break;
            default: return {}; 
        }
        move.set_flags(promoteFlag);
    }

    int input[4] = {from_file, from_rank, to_file, to_rank};

    for (auto i: input) 
    {
        if (i < 0 || i >= 8) 
        {
            return {};
        }
    }
    move.set_from(static_cast<Square>(from_rank * 8 + from_file));
    move.set_to(static_cast<Square>(to_rank * 8 + to_file));
    Piece pce = pos.get_piece(move.from());
    Piece captured = pos.get_piece(move.to());

    if (captured != Piece::EMPTY) move.switch_flag_to_capture();
    if (pce == Piece::EMPTY) return {};

    if (pce == Piece::KING && move.from() == Square::E1 && move.to() == Square::G1) 
    {
        move.set_flags(Flag::KING_CASTLE);
    }
    if (pce == Piece::KING && move.from() == Square::E8 && move.to() == Square::G8) 
    {
        move.set_flags(Flag::KING_CASTLE);
    }

    if (pce == Piece::KING && move.from() == Square::E1 && move.to() == Square::C1) 
    {
        move.set_flags(Flag::QUEEN_CASTLE);
    }
    if (pce == Piece::KING && move.from() == Square::E8 && move.to() == Square::C8) 
    {
        move.set_flags(Flag::QUEEN_CASTLE);
    }

    return move;

}

std::vector<std::string> split(std::string str, char split)
{
    std::string word = "";
    std::vector<std::string> result;
    for (auto i : str)
    {
        if (i == split && word != "")
        {
            result.push_back(word);
            word = "";
            continue;
        }
        word += i;
    }

    if (!word.empty())
    {
        result.push_back(word);
    }
    return result;

}

void uci_loop()
{
    while (true)
    {
        std::string input;
        std::cin >> input;
        auto args = split(input, ' ');

        if (args.empty()) continue;

        if (args.at(0).compare("uci") == 0) std::cout << "id name " << engine_version << '\n' << "uciok";
    
        else if (args.at(0).compare("isready") == 0) std::cout << "readyok";

        else if (args.at(0).compare("quit") == 0) break;

        if (args.size() == 2)
        {

        }



    }
}


    
}
