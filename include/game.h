#pragma once

#include "snake.h"
#include "apple.h"

#include <RedGir/engine.h>
#include <RedGir/timer.h>

#include <cstdint>
#include <vector>

// IDEA Maybe have some sort of stack to keep track of game state order?
// The only real reason I would want that is to easily keep track of
// previous state when entering the pause menu. Would I even necessarily
// need game menus to be opened and closed in stack order?
enum class GameState {
    MainMenu,
    Pause,
    Playing,
    Options,
    GameOver,
};

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void loadGameScene();

    void loadOptions();
    void removeOptions();
    void loadMainMenu();
    void removeMainMenu();
    void loadPauseMenu();
    void removePauseMenu();
    void loadGameOver();
    void removeGameOver();

    void update();
    void updateOptions();
    void updatePaused();
    void updateMainMenu();
    void updatePlaying();
    void updateGameOver();

    void clearScene();
    std::vector<GameEvent> updateSnake();
    GameContext generateGameContext();

    void resetGameData();

    void loadSpriteSheets();

private:
    std::vector<Snake> m_snakeParts;
    Apple m_apple;
    Engine m_engine;
    GameState m_state;
    float m_lastSnakeUpdate;
    float m_snakeSpeed;
    uint32_t m_applesConsumed;
    GameState m_stateBeforeOptions;

    SpriteSheetId m_optionsId;
    SpriteId m_options;
    SpriteSheetId m_mainMenuId;
    SpriteId m_mainMenu;
    SpriteSheetId m_pauseMenuId;
    SpriteId m_pauseMenu;
    SpriteSheetId m_gameOverId;
    SpriteId m_gameOver;
};
