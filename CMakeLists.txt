cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 and SDL2_ttf libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Source files
set(SOURCES
    main.cpp
)

# Executable
add_executable(SnakeGame ${SOURCES})

# Link libraries
target_link_libraries(SnakeGame
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)

# Copy assets (font file) to the build directory
file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)