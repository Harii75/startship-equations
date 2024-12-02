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

# Add executable target
add_executable(startship-equations ${SOURCES})

# Specify the output directory for the executable
set_target_properties(startship-equations PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Copy the data directory to the root folder (next to the executable)
add_custom_command(
    TARGET startship-equations POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data ${CMAKE_SOURCE_DIR}/data
)

# Debugging message
message(STATUS "The executable will be generated in the root folder.")
