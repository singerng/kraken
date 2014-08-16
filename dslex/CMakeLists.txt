# Some parts taken from zaita

cmake_minimum_required(VERSION 2.8)
project(dslex)

# Set flags / options
# Use for libraries
# set(COMPILE_OPTIONS "-Wall -fPIC")
# set(LINK_OPTIONS "-shared")

# For executables
set(COMPILE_OPTIONS "-O2") # Optimize
set(LINK_OPTIONS " ")

set(EXE_NAME "dslex")

# Add include directory
include_directories("${CMAKE_HOME_DIRECTORY}/include")
# Find all our source files
file(GLOB_RECURSE sourceFiles ${CMAKE_HOME_DIRECTORY}/src/*.cpp ${CMAKE_HOME_DIRECTORY}/src/*.c)

# Compile and link
add_executable(${EXE_NAME} ${sourceFiles})
set_target_properties(${EXE_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_OPTIONS} LINK_FLAGS ${LINK_OPTIONS})
