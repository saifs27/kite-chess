cmake_minimum_required(VERSION 3.22.1)

project(
    KiteChess
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

add_subdirectory(src)
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
