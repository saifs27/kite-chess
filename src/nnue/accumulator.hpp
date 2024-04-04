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

#pragma once
#include <array>
#include <vector>
#include "../search.hpp"
#include "../types.hpp"
#include "layers.hpp"
#include "misc.hpp"
namespace Kite::NNUE {
struct LinearLayer
{
    std::array<Node, 32> data;
    LinearLayer(std::array<Node, 32> input) : data(input) {};
    Node at(int i) const {return data.at(i);}

};
struct Accumulator
{
    std::array<std::array<float, size::M>, 2> data;
    float at(Color side, int n) const {return data.at(static_cast<int>(side)).at(n);}
    void refresh(
        const LinearLayer& layer, // L_0
        Accumulator& new_acc, //store result
        const std::vector<int>& active_features, 
        Color side
    );
    void update(
        const LinearLayer& layer, 
        Accumulator& new_acc, 
        const std::vector<int>& removed_features,
        const std::vector<int>& added_features,
        Color side);
};

}