cmake_minimum_required(VERSION 3.10)
project(BrickPuzzle)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(/utf-8)

add_executable(brick-puzzle main.cpp 
                Board.h Part.h
            )
