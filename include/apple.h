#pragma once

#include "utils.h"

class Apple {
public:
    Apple();
    ~Apple();

    GameEvent update(GameContext &context);

private:
    Coords m_coords;
};
