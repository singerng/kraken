cmake_minimum_required(VERSION 2.8)
project(lex)

# Add include directory
include_directories("include")

# Compile and link
add_library(lex src/lex.cpp src/dfa.cpp)

# Require some stuff we need
target_compile_features(lex PRIVATE cxx_range_for)

add_subdirectory("lex")