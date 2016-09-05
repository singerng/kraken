cmake_minimum_required(VERSION 3.5)
project(kraken)

add_definitions(-std=c++11)

add_subdirectory("parse")

# Compile the DFA
add_custom_command(OUTPUT kraken.dfa COMMAND lexgen ARGS src/kraken.lex ${CMAKE_CURRENT_BINARY_DIR}/kraken.dfa
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS src/kraken.lex)
add_custom_target(dfa DEPENDS kraken.dfa)

# Move the test files into the build area
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(kraken src/main.cpp)

add_dependencies(kraken dfa)

# Require some stuff we need
target_compile_features(lexgen PRIVATE cxx_range_for)
include_directories("parse/include")

target_link_libraries(kraken parse)