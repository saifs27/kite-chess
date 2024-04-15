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

void Accumulator::refresh (const Layer<size::M, size::N>& layer, 
                           Accumulator& new_acc, 
                           const std::vector<int>& active_features, 
                           Color side)
{
    int perspective = static_cast<int>(side);
    for (int i = 0; i < size::M; i++)
    {
        auto bias = layer.bias;
        new_acc(perspective, i) = bias.at(i, 0);
    }

    for (int a: active_features)
    {
        for (int i = 0; i < size::M; i++)
        {
            auto weight = layer.weight;
            new_acc(perspective, i) += weight(a, i);
        }
    }
}

    void update(const Layer<size::M, size::N>& layer, 
                Accumulator& new_acc, 
                Accumulator& prev_acc, 
                const std::vector<int>& removed_features, 
                const std::vector<int>& added_features, 
                Color side)
    {
        int perspective = static_cast<int>(side);
        // copy previous values
        for (int i = 0; i < size::M; i++)
        {
            new_acc(perspective, i) = prev_acc(perspective, i);
        }

        for (int f : removed_features)
        {
            for (int i = 0; i < size::M; i++)
            {
                auto weight = layer.weight;
                new_acc(perspective, i) -= weight(f, i);
            }
        }

        for (int f : added_features)
        {
            for (int i = 0; i < size::M; i++)
            {
                auto weight = layer.weight;
                new_acc(perspective, i) += weight(f, i);
            }

        }


    }


















}