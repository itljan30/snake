# Introduction
This is a demonstration of [RedGir](https://github.com/itljan30/RedGir), a game engine that I have made in Rust.
Calling it a game engine might be a bit of a stretch, it is intended as an abstraction over OpenGL and GLFW in order 
to simplify window management, rendering, shader handling, and input processing.

## Pre-requisites
- [Rust](https://www.rust-lang.org/tools/install): To build the game engine

## Building
```sh
# clone the repository
git clone https://github.com/itljan30/snake.git
cd snake

# make the build directory
mkdir build
cd build

# call CMake
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
The game should now be in the `bin` directory found in the root of the project
