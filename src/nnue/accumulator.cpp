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
#include "accumulator.hpp"

namespace Kite::NNUE {

void Accumulator::refresh (const Layer& layer, Accumulator& new_acc, const std::vector<int>& active_features, Color side)
{
    int perspective = static_cast<int>(side);
    for (int i = 0; i < size::M; i++)
    {
        new_acc(perspective, i) = layer.
    }

    for (int i : active_features)
    {
        
    }
}

}