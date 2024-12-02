cmake_minimum_required(VERSION 3.10)

# Project name and language
project(startship-equations VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include lib)

# Gather all source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "lib/tinyxml2.cpp")

# Add the executable target
add_executable(startship-equations ${SOURCES})

# Copy the data directory to the build directory
add_custom_command(
    TARGET startship-equations POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:startship-equations>/data
)
