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
#include <algorithm>
#include <array>

#include "matrix.hpp"

namespace Kite::NNUE {

template <std::size_t N, std::size_t M>
struct Layer {
    Matrix<float, Order::COLUMN_MAJOR, N, M> weight;
    Matrix<float, Order::COLUMN_MAJOR, N, 1> bias;
    int num_inputs = N;
    int num_outputs;
    Layer(const std::array<float, 32>& features);
};

struct ClippedRELU {
    /*
    A modified RELU where

    f(x) = x for x >= 0 and x =< 1
         = 1 for x > 1
         = 0 for x < 0
    */
    std::vector<float> output = {};

    ClippedRELU(int size, const std::vector<float>& input);
    void set_relu(int size, const std::vector<float>& input);
};

void clipped_relu(int size, const std::vector<float>& input,
                  const std::vector<float>& output);

}  // namespace Kite::NNUE