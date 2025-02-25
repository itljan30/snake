#include "game.h"
#include "snake.h"
#include "apple.h"

#include <RedGir/engine.h>

Engine initializeEngine() {
    return Engine::create()
        .setWindowName("Snake")
        .setWindowSize(800, 600)
        .setClearColor(Color(32, 32, 32, 255))
        .pollKeyboard()
        .notResizable()
        .init();
}

Game::Game() : m_engine(initializeEngine()) {}

Game::~Game() {}

void Game::update() {
    // I feel like there's probably a way better way to share this information, but I have no idea.
    GameContext context {
        .snakeParts = m_snakeParts,
        .engine = m_engine,
        .apple = m_apple,
    };

    std::vector<GameEvent> events;

    for (Snake &snakePart : m_snakeParts) {
        events.push_back(snakePart.update(context));
    }

    for (GameEvent event : events) {
        switch (event) {
            case None: break;
            case AppleConsumed: {
                // delete current apple and create a new one in an unoccupied space
                break;
            }
            case GameOver: {
                // show game over message
                break;
            }
        }
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
