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
    // TODO Add error handling for failing to create sprites.
    static void addNewSnakeToSnakeParts(GameContext &context);
    Snake(SpriteId id, Coords coords, Snake *head, Direction direction, bool hasMoved);
    ~Snake();

    Coords getCoords();
    void addSnake(GameContext &context);
    GameEvent update(GameContext &context);
    void removeFromEngine(GameContext &context);

    bool operator==(const Snake &other) const {
        return (m_id == other.m_id);
    }

private:
    void updateDirection(GameContext &context);
    GameEvent updatePosition(GameContext &context);
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
