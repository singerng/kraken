cmake_minimum_required(VERSION 2.8)
project(dslex)

# Add include directory
include_directories("include")

# Compile and link
add_executable(dslex src/main.cpp src/regex.cpp src/dfa.cpp)

# Require some stuff we need
target_compile_features(dslex PRIVATE cxx_range_for)