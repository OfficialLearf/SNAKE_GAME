cmake_minimum_required(VERSION 4.0)
project(SNAKE_GAME)

set(CMAKE_CXX_STANDARD 23)

add_executable(SNAKE_GAME main.cpp)
find_package(raylib CONFIG REQUIRED)
target_link_libraries(SNAKE_GAME PRIVATE raylib)