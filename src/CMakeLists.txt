
add_executable(kite
                main.cpp types.hpp rays.hpp perft.hpp
                bitboards.cpp bitboards.hpp 
                position.cpp position.hpp
                search.cpp search.hpp 
                movegen.cpp movegen.hpp 
                uci.cpp uci.hpp
                moves.cpp moves.hpp
                evaluation.cpp evaluation.hpp
                nnue/matrix.cpp
                nnue/layers.cpp 
                nnue/accumulator.cpp
                )
target_compile_features(kite PRIVATE cxx_std_20)

target_include_directories(kite PRIVATE src)

