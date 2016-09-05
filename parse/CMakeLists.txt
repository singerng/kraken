cmake_minimum_required(VERSION 2.8)
project(parse)

# Add include directory
include_directories("include")

# Compile and link
add_library(parse src/lex.cpp src/dfa.cpp)

# Require some stuff we need
target_compile_features(parse PRIVATE cxx_range_for)

add_subdirectory("lex")
add_subdirectory("parse")