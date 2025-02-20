#include "game.h"
#include "entity.h"

#include <RedGir/engine.h>

Engine initializeEngine() {
    return Engine::create()
        .setWindowName("Snake")
        .setWindowSize(800, 600)
        .setClearColor(Color(0, 0, 0, 255))
        .pollKeyboard()
        .notResizable()
        .init();
}

Game::Game() : m_engine(initializeEngine()) {}

void Game::update() {
    for (Entity entity : m_entities) {
        entity.update();
    }
}

void Game::run() {
    while (m_engine.isRunning()) {
        update();

        m_engine.drawFrame();

        if (m_engine.getKeyState(Key::Escape) == Action::Pressed) {
            m_engine.stop();
        }
    }
}
