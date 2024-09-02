cmake_minimum_required(VERSION 3.5)
project(CHIP8-EMU)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(CHIPPY CHIPPY/main.cpp
        CHIPPY/src/Renderer.cpp
        CHIPPY/include/Processor.h
        CHIPPY/src/Processor.cpp)

target_link_libraries(CHIPPY ${SDL2_LIBRARIES})