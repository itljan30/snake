#pragma once

#include <RedGir/engine.h>

#include <cstdint>
#include <vector>

class Snake;
class Apple;

struct Coords {
    int32_t x;
    int32_t y;

    bool operator==(const Coords &other) const {
        return (x == other.x && y == other.y);
    }
};

struct GameContext {
    std::vector<Snake> &snakeParts;
    Engine &engine;
    Apple &apple;
};

enum class GameEvent {
    None,
    AppleConsumed,
    GameOver,
};
