#pragma once

#include "entity.h"

#include <RedGir/engine.h>
#include <RedGir/timer.h>

#include <vector>

class Game {
public:
    Game();

    void run();

private:
    void update();

private:
    std::vector<Entity> m_entities;
    Engine m_engine;
};
