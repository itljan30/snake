#pragma once

#include <RedGir/engine.h>

#include <cstdint>
#include <vector>

class Snake;
class Apple;

struct Coords {
    int32_t x;
    int32_t y;
};

struct GameContext {
    const std::vector<Snake> &snakeParts;
    Engine &engine;
    const Apple &apple;
};

enum GameEvent {
    None,
    AppleConsumed,
    GameOver,
};
