cmake_minimum_required(VERSION 2.8)

# Add include directory
include_directories("include")

# Compile and link
add_executable(lexgen src/main.cpp src/regex.cpp)

# Require some stuff we need
target_compile_features(lexgen PRIVATE cxx_range_for)

set_target_properties(lexgen PROPERTIES OUTPUT_NAME lex)

target_link_libraries(lexgen lex)