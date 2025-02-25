#pragma once

#include "snake.h"
#include "apple.h"

#include <RedGir/engine.h>
#include <RedGir/timer.h>

#include <vector>

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void update();

private:
    std::vector<Snake> m_snakeParts;
    Apple m_apple;
    Engine m_engine;
};
