cmake_minimum_required(VERSION 3.22.1)

project(
    SmyslovEngine
    VERSION 1.0
    DESCRIPTION "A chess engine"
    LANGUAGES CXX)
    # Add local project module path so CMake includes custom CMake modules.
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")


if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)
    include(CTest)
endif()

enable_testing()
include(FetchContent)

add_executable(smyslov
                src/main.cpp
                src/bitboards.cpp
                src/bitboards.hpp 
                src/position.cpp
                src/position.hpp
                src/magic.cpp
                src/magic.hpp
                src/types.hpp
                src/search.cpp
                src/search.hpp 
                src/movegen.cpp
                src/movegen.hpp 
                src/rays.cpp
                src/rays.hpp
                )

target_include_directories(smyslov PRIVATE src)


if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
