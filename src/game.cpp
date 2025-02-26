#include "game.h"
#include "snake.h"
#include "apple.h"
#include "constants.h"

#include <RedGir/engine.h>

#include <cstdint>

Engine initializeEngine() {
    Engine engine = Engine::create()
        .setWindowName("Snake")
        .setWindowSize(g_screenWidth, g_screenHeight)
        .setClearColor(Color(16, 16, 16, 255))
        .pollKeyboard()
        .notResizable()
        .init();

    engine.setFps(g_targetFps);

    return engine;
}

Game::Game()
    : m_engine(initializeEngine()), m_state(GameState::Playing), m_snakeSpeed(3), m_apple(Apple::dummyApple()) {
    m_lastSnakeUpdate = m_engine.timeSinceInitializationSeconds();
}

std::vector<GameEvent> Game::updateSnake() {
    GameContext context = generateGameContext();

    std::vector<GameEvent> events;

    // Iterate backwards to simplify position updates.
    for (int32_t i = m_snakeParts.size() - 1; i >= 0; i--) {
        Snake &snakePart = m_snakeParts.at(i);
        events.push_back(snakePart.update(context));
    }

    return std::move(events);
}

Game::~Game() {}

void Game::updatePlaying() {
    if (m_engine.getKeyState(Key::Escape) == Action::Pressed) {
        m_state = GameState::Pause;
        return;
    }

    // Cap snake speed so it doesn't move 60 times per second.
    float dt =  m_engine.timeSinceInitializationSeconds() - m_lastSnakeUpdate;
    if (dt < 0.25 / m_snakeSpeed) {
        return;
    }

    // I don't really like this GameEvent system not gonna lie, it feels like a very ductape solution.
    std::vector<GameEvent> events = updateSnake();

    for (GameEvent event : events) {
        switch (event) {
            case GameEvent::None: {
                break;
            }
            case GameEvent::AppleConsumed: {
                GameContext context = generateGameContext();
                m_apple.removeFromEngine(context);
                
                m_apple = Apple::createNewApple(context);

                if (m_applesConsumed == 4) {
                    m_snakeSpeed += 1;
                }

                Snake::addNewSnakeToSnakeParts(context);

                m_applesConsumed = (m_applesConsumed + 1) % 5;
                break;
            }
            case GameEvent::GameOver: {
                m_state = GameState::GameOver;
                return;
                // TODO load game over screen and set game state to game over
            }
        }
    }

    m_lastSnakeUpdate = m_engine.timeSinceInitializationSeconds();
}

void Game::updatePaused() {
    if (m_engine.getKeyState(Key::X) == Action::Pressed) {
        // TODO clear game assets and load main menu
    }
    else if (m_engine.getKeyState(Key::O) == Action::Pressed) {
        // TODO load options menu
        // don't clear game assets so game is in background
    }
    else if (m_engine.getKeyState(Key::Escape) == Action::Pressed) {
        // TODO remove assets for pause menu 
        // enter playing state
    }
}

void Game::updateMainMenu() {
    if (m_engine.getKeyState(Key::X) == Action::Pressed) {
        m_engine.stop();
    }
    else if (m_engine.getKeyState(Key::O) == Action::Pressed) {
        // TODO load options menu
        // I think I specifically don't clear the main menu stuff so that options display above the main menu
    }
    else if (m_engine.getKeyState(Key::Enter) == Action::Pressed) {
        // TODO load main game
        // delete main menu assets
    }
}

void Game::updateOptions() {
    if (m_engine.getKeyState(Key::Escape) == Action::Pressed) {
        // TODO return to previous game state.
        return;
    }
    // TODO probably have some number keys to adjust screen resolution or something
}

GameContext Game::generateGameContext() {
    // IDEA I feel like there's probably a way better way to share this information, but I have no idea.
    // Maybe a smart pointer of some kind?
    return GameContext {
        .snakeParts = m_snakeParts,
        .engine = m_engine,
        .apple = m_apple,
    };
}

void Game::updateGameOver() {
}

void Game::update() {
    // IDEA This feels like it could become cumbersome with more states, is there a better way to do this?
    // ooh maybe a hashmap with key being state and value being callback function?
    switch (m_state) {
        case GameState::MainMenu: {
            updateMainMenu();
            break;
        }
        case GameState::Pause: {
            updatePaused();
            break;
        }
        case GameState::Playing: {
            updatePlaying();
            break;
        }
        case GameState::Options: {
            updateOptions();
            break;
        }
        case GameState::GameOver: {
            updateGameOver();
            // TODO do something
            break;
        }
    }
}

void Game::loadGameScene() {
    GameContext context = generateGameContext();
    m_apple = Apple::createNewApple(context);
    Snake::addNewSnakeToSnakeParts(context);
}

void Game::addNewSnake() {
}

void Game::run() {
    // TODO set up crt and scanlines shader
    // TODO set up main menu scene

    loadGameScene();

    while (m_engine.isRunning()) {
        update();
        m_engine.drawFrame();
    }
}
