cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

# Find SDL2 and SDL2_ttf
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Define the executable
add_executable(SnakeGame main.cpp)

# Include and link SDL2 + SDL2_ttf properly
target_include_directories(SnakeGame PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(SnakeGame PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})

# Ensure linking works
target_link_libraries(SnakeGame PRIVATE SDL2 SDL2_ttf)
