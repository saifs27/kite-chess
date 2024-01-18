#pragma once
#include "types.hpp"
#include "position.hpp"
#include "movegen.hpp"

namespace Smyslov {

class TranspositionTable;
class ThreadPool;

namespace Search {
struct Stack {
    int ply;
    Move current;
    Move excluded;

};
        

}

}