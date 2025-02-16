cmake_minimum_required(VERSION 3.15)
project(snake)

add_executable(snake
    src/main.cpp
)

target_include_directories(snake PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
