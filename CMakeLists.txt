# https://medium.com/heuristics/c-application-development-part-3-cmakelists-txt-from-scratch-7678253e5e24

# cmake version to be used
cmake_minimum_required( VERSION 3.0 )

# project name
project( chess )

# flags

# files

# include
include_directories( ./include ./src )

# target
add_executable( chess ./src/chess.cpp ./src/cpu.cpp )

