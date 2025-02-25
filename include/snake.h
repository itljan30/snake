#pragma once

#include "utils.h"

#include <RedGir/sprite.h>

enum class Direction {
    Up,
    Left,
    Right,
    Down,
    None,
};

class Snake {
public:
    static void addNewSnakeToSnakeParts(GameContext &context);
    Snake(SpriteId id, bool moved, Coords coords, Snake *head, Direction direction);
    ~Snake();

    void addSnake(GameContext &context);
    GameEvent update(GameContext &context);

private:

    void updateDirection(GameContext &context);
    void updatePosition(GameContext &context);
    GameEvent resolveCollisions(GameContext &context);

private:
    // If there's a better / more C++ way to do this rather than raw C pointers, let me know.
    // I could probably have shared_ptr, but that sounds like it's overcomplicating things.
    Snake *m_head;
    Coords m_coords;
    Direction m_direction;
    SpriteId m_id;
    bool m_hasMoved;
};
