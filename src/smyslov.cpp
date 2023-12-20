#include <iostream>
#include "board.hpp"

int main() {
    U64 result = knightAttacks(D4);
    std::cout << result << '\n';
}