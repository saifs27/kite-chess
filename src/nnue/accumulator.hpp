#pragma once
#include <array>

struct NNUEAcumulator
{
    std::array<std::array<float, 64>, 2> v;
    void refresh();
    void update();
};