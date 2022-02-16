cmake_minimum_required(VERSION 3.21)
project(minecraft C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "-O3 -Wall -Wpedantic -Wno-gnu-zero-variadic-macro-arguments")

file(GLOB SOURCES
        "src/main/*.c"
        "src/main/game/*.c"
        "src/main/engine/*.c"
        "src/main/engine/*/*.c"
        "src/main/engine/*/*/*.c"
        )


add_link_options(-lm -ldl -lpthread)
add_executable(minecraft ${SOURCES})
