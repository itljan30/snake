# TODO Compile all libraries and exe into `${CMAKE_SOURCE_DIR}/bin/`

cmake_minimum_required(VERSION 3.15)
project(snake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    snake
    src/main.cpp
    src/game.cpp
    src/snake.cpp
    src/apple.cpp
)

target_include_directories(snake PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)

FetchContent_Declare(
    RedGir
    GIT_REPOSITORY https://github.com/itljan30/RedGir.git
    GIT_TAG        v0.1.0
)

FetchContent_MakeAvailable(RedGir)

add_dependencies(snake RedGir_cpp)

target_link_libraries(snake PRIVATE RedGir_cpp)
