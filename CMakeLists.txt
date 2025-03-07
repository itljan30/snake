# TODO Compile all libraries, exe, and assets dir into `${CMAKE_SOURCE_DIR}/bin/`

cmake_minimum_required(VERSION 3.15)
project(snake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

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

add_custom_command(
    TARGET ${CMAKE_SOURCE_DIR}/bin/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_SOURCE_DIR}/bin/assets
)
