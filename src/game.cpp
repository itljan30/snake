#include "game.h"
#include "snake.h"
#include "apple.h"
#include "constants.h"

#include <RedGir/engine.h>

#include <cstdint>

static const SpriteId dummySprite = SpriteId { INT32_MAX };
static const SpriteSheetId dummySpriteSheet = SpriteSheetId { INT32_MAX };
static const int32_t startingSpeed = 3;

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

void Game::resetGameData() {
    m_snakeParts.clear();
    m_snakeSpeed = startingSpeed;
}

Game::Game()
    : m_engine(initializeEngine()), m_state(GameState::MainMenu),
      m_apple(Apple::dummyApple()), m_stateBeforeOptions(GameState::MainMenu),
      m_mainMenu(dummySprite), m_pauseMenu(dummySprite), m_gameOver(dummySprite),
      m_options(dummySprite), m_mainMenuId(dummySpriteSheet), m_pauseMenuId(dummySpriteSheet),
      m_gameOverId(dummySpriteSheet), m_optionsId(dummySpriteSheet) 
    {
    // HACK Reserve is to prevent Snake objects from moving in memory 
    // since a Snake object holds a pointer to the Snake in front of it.
    m_snakeParts.reserve(g_tilesPerRow * g_tilesPerCol);
    m_lastSnakeUpdate = m_engine.timeSinceInitializationSeconds();
}

std::vector<GameEvent> Game::updateSnake() {
    // I don't really like this GameEvent system not gonna lie, it feels like a very hacky solution.
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
        loadPauseMenu();
        m_state = GameState::Pause;
        return;
    }

    // Cap snake speed so it doesn't move 60 times per second.
    float dt =  m_engine.timeSinceInitializationSeconds() - m_lastSnakeUpdate;
    if (dt < 0.25 / m_snakeSpeed) {
        return;
    }

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

                if (m_applesConsumed == 19) {
                    m_snakeSpeed += 1;
                }

                Snake::addNewSnakeToSnakeParts(context);

                m_applesConsumed = (m_applesConsumed + 1) % 20;
                break;
            }
            case GameEvent::GameOver: {
                loadGameOver();
                m_state = GameState::GameOver;
                return;
            }
        }
    }

    m_lastSnakeUpdate = m_engine.timeSinceInitializationSeconds();
}

void Game::updatePaused() {
    if (m_engine.getKeyState(Key::X) == Action::Pressed) {
        clearScene();
        removePauseMenu();
        loadMainMenu();
        m_state = GameState::MainMenu;
    }
    else if (m_engine.getKeyState(Key::O) == Action::Pressed) {
        m_stateBeforeOptions = GameState::Pause;
        removePauseMenu();
        loadOptions();
        m_state = GameState::Options;
    }
    else if (m_engine.getKeyState(Key::Escape) == Action::Pressed) {
        removePauseMenu();
        m_state = GameState::Playing;
    }
}

void Game::updateMainMenu() {
    if (m_engine.getKeyState(Key::X) == Action::Pressed) {
        m_engine.stop();
    }
    else if (m_engine.getKeyState(Key::O) == Action::Pressed) {
        m_stateBeforeOptions = GameState::MainMenu;
        removeMainMenu();
        loadOptions();
        m_state = GameState::Options;
    }
    else if (m_engine.getKeyState(Key::Enter) == Action::Pressed) {
        removeMainMenu();
        resetGameData();
        loadGameScene();
        m_state = GameState::Playing;
    }
}

void Game::updateOptions() {
    if (m_engine.getKeyState(Key::Escape) == Action::Pressed) {
        removeOptions();
        m_state = m_stateBeforeOptions;
        switch (m_stateBeforeOptions) {
            case GameState::MainMenu: {
                loadMainMenu();
                break;
            }
            case GameState::Pause: {
                loadPauseMenu();
                break;
            }

            // These should be impossible
            case GameState::Options: break;
            case GameState::Playing: break;
            case GameState::GameOver: break;
        }
        return;
    }
    // TODO probably have some number keys to adjust screen resolution or something
}

GameContext Game::generateGameContext() {
    // IDEA I feel like there's probably a way better way to share this information, but I have no idea.
    // Maybe a smart pointer of some kind?
    return GameContext {
        m_snakeParts,
        m_engine,
        m_apple,
    };
}

void Game::updateGameOver() {
    if (m_engine.getKeyState(Key::Enter) == Action::Pressed) {
        removeGameOver();
        clearScene();
        loadGameScene();
        m_state = GameState::Playing;
    }
    if (m_engine.getKeyState(Key::X) == Action::Pressed) {
        removeGameOver();
        clearScene();
        loadMainMenu();
        m_state = GameState::MainMenu;
    }
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
            break;
        }
    }
}

void Game::clearScene() {
    GameContext context = generateGameContext();

    m_apple.removeFromEngine(context);
    for (Snake &snake : m_snakeParts) {
        snake.removeFromEngine(context);
    }

    resetGameData();
}

void Game::loadGameScene() {
    GameContext context = generateGameContext();
    m_apple = Apple::createNewApple(context);
    Snake::addNewSnakeToSnakeParts(context);
}

void Game::removeOptions() {
    m_engine.removeSprite(m_options);
}

void Game::loadOptions() {
    m_options = m_engine.addSprite(
        m_optionsId, 0,
        0, 0, 10,
        800, 600,
        m_engine.defaultShader()
    );
}

void Game::removeGameOver() {
    m_engine.removeSprite(m_gameOver);
}

void Game::loadGameOver() {
    m_gameOver = m_engine.addSprite(
        m_gameOverId, 0,
        0, 0, 10,
        800, 600,
        m_engine.defaultShader()
    );
}

void Game::removePauseMenu() {
    m_engine.removeSprite(m_pauseMenu);
}

void Game::loadPauseMenu() {
    m_pauseMenu = m_engine.addSprite(
        m_pauseMenuId, 0,
        0, 0, 10,
        800, 600,
        m_engine.defaultShader()
    );
}

void Game::removeMainMenu() {
    m_engine.removeSprite(m_mainMenu);
}

void Game::loadMainMenu() {
    m_mainMenu = m_engine.addSprite(
        m_mainMenuId, 0,
        0, 0, 10,
        800, 600,
        m_engine.defaultShader()
    );
}

void Game::loadSpriteSheets() {
    // This is very fragile since it requires the exe to be in build/
    // What is the best way to have it search for these files?
    m_mainMenuId = m_engine.addSpriteSheet("../assets/main-menu.png", 800, 600);
    m_pauseMenuId = m_engine.addSpriteSheet("../assets/paused.png", 800, 600);
    m_gameOverId = m_engine.addSpriteSheet("../assets/game-over.png", 800, 600);
    m_optionsId = m_engine.addSpriteSheet("../assets/options.png", 800, 600);
}

void Game::run() {
    // I was going to have some cool shaders that made it look more retro with some scanlines and a curved edges,
    // but converting the shader creation system over to C++ has turned out to be quite difficult. It uses callback
    // functions in order to get the appropriate attribute and uniform information to send to the GPU, but because of
    // some conversions that need to be made in order to export C functions that are used by the C++ api, I would need
    // to dynamically create function pointers that inherently know which function to pass the converted types to. From
    // what I've found though, I can't really have the engine store thouse dynamically created functions as function 
    // pointers. I think I'm going to have to completely redesign the system in order to make it possible.

    loadSpriteSheets();
    loadMainMenu();

    while (m_engine.isRunning()) {
        update();
        m_engine.drawFrame();
    }
}
