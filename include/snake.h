#pragma once

#include "utils.h"

enum class Direction {
    Up,
    Left,
    Right,
    Down,
};

class Snake {
public:
    Snake(GameContext &context);
    ~Snake();

    GameEvent update(GameContext &context);

private:
    void updateDirection(GameContext &context);
    void updatePosition(GameContext &context);
    GameEvent resolveCollisions(GameContext &context);

private:
    // If there's a better / more C++ way to do this rather than raw C pointers, let me know.
    Snake *m_head;
    Coords m_coords;
    Direction m_direction;
    SpriteId m_id;
};
