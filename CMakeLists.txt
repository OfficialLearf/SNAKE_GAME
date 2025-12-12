cmake_minimum_required(VERSION 4.0)
project(SNAKE_GAME)

set(CMAKE_CXX_STANDARD 23)

add_executable(SNAKE_GAME main.cpp)
find_package(raylib CONFIG REQUIRED)
target_link_libraries(SNAKE_GAME PRIVATE raylib)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXE_LINKER_FLAGS "-static")
target_compile_options(
        SNAKE_GAME  # <-- Replace 'MyProject' with your actual target name
        PRIVATE
        # Comprehensive Warnings
        -Wall
        -Wextra
        -Wpedantic

        # Treat Warnings as Errors (Code must be perfect to compile)
        -Werror

        # Other useful modern warnings (Optional but recommended)
        -Wshadow        # Warn when a local variable shadows a global/member variable
        -Wconversion    # Warn about implicit type conversions (e.g., int to char)
        -Wnon-virtual-dtor # Warn if a base class has a virtual function but non-virtual destructor
)
