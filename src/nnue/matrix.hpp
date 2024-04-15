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
namespace Kite::NNUE {


enum class Order {ROW_MAJOR, COLUMN_MAJOR};

template <typename T, Order U, std::size_t Rows, std::size_t Cols>
struct Matrix
{
    T at(int row_index, int column_index)
    {
        int index = (U == Order::ROW_MAJOR) ? (column_index + row_index * Rows) : (row_index + Cols * column_index);
        return data.at(index);    
    }
    Matrix(){std::fill(data.begin(), data.end(), 0);}
    void assign(int i, int j, T input) {}
    T& operator()(int i, int j){return data.at(index());}
    Matrix operator*(const Matrix& M);
    Matrix operator*(const Matrix<T, U, Cols, 1>& M);

    private:
    T index(int row_index, int column_index) 
    {
        if constexpr (U == Order::ROW_MAJOR) return column_index + row_index * Rows;
        return row_index + Cols * column_index;
    }
    std::array<T, Rows*Cols> data;

};


}