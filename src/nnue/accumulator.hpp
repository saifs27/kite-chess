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
#include "matrix.hpp"
#include "misc.hpp"
namespace Kite::NNUE {

struct Accumulator {
    Matrix<float, Order::COLUMN_MAJOR, size::M, 2> data;
    float& operator()(int i, int j) { return data(i, j); }

    void refresh(const Layer<size::M, size::N>& layer,  // L_0
                 Accumulator& new_acc,                  // store result
                 const std::vector<int>& active_features, Color side);
    void update(const Layer<size::M, size::N>& layer, Accumulator& new_acc,
                Accumulator& prev_acc, const std::vector<int>& removed_features,
                const std::vector<int>& added_features, Color side);
};

}  // namespace Kite::NNUE