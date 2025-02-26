#pragma once

#include "utils.h"

#include <RedGir/sprite.h>

class Apple {
public:
    // TODO Add error handling for failing to create sprites.
    static Apple createNewApple(GameContext &context);
    static Apple dummyApple();
    ~Apple();

    Coords getCoords();
    void removeFromEngine(GameContext &context);
    GameEvent update(GameContext &context);

private:
    Apple(Coords coords, SpriteId id);

private:
    Coords m_coords;
    SpriteId m_id;
};
