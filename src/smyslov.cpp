#include <iostream>
#include "bitboards.hpp"

int main() {

    U64 rookDirection = rookAttacks(D4);

    std::cout << rookDirection << '\n';

}