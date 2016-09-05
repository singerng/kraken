cmake_minimum_required(VERSION 2.8)

# Add include directory
include_directories("include")
include_directories("include")

# Compile and link
add_executable(parsegen src/main.cpp)

# Require some stuff we need
target_compile_features(parsegen PRIVATE cxx_range_for)

set_target_properties(parsegen PROPERTIES OUTPUT_NAME parse)

target_link_libraries(parsegen parse)