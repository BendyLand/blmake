# CMakeLists.txt generated from blmake.lua!

# Minimum CMake version
cmake_minimum_required(VERSION 3.14)

# Project metadata
project(main VERSION 0.0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler options
set(CMAKE_CXX_COMPILER "g++")

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCE_FILES
	main.cpp
	utils.cpp
)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Compilation flags
set(COMPILATION_FLAGS "-Wall -Werror -DDEBUG -pg -O2 -flto -m64")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMPILATION_FLAGS "-g")
endif()

# Add executable
add_executable(main ${SOURCE_FILES})

# Apply compilation flags
target_compile_options(main PRIVATE ${COMPILATION_FLAGS})

# Add framework path to cmake prefix path
set(CMAKE_PREFIX_PATH "")

# Find required package 
find_package( REQUIRED COMPONENTS )

# Link libraries
target_link_libraries(main PRIVATE 
	m
	pthread
)

# Custom linker paths
link_directories(${CMAKE_SOURCE_DIR}/libs)

# Pre-build and post-build hooks
add_custom_command(TARGET main PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running pre-build script..."
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/pre_build.sh
)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running post-build script..."
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/post_build.sh
)