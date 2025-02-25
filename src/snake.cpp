#include "snake.h"
#include "utils.h"
#include "constants.h"

#include <RedGir/engine.h>

Snake::Snake(GameContext &context) {
    
} 

Snake::~Snake() {}

void Snake::updateDirection(GameContext &context) {
    if (m_head == NULL) {
        if (context.engine.getKeyState(Key::ArrowDown) == Action::Pressed && m_direction != Direction::Up) {
            m_direction = Direction::Down;
        }
        else if (context.engine.getKeyState(Key::ArrowRight) == Action::Pressed && m_direction != Direction::Left) {
            m_direction = Direction::Right;
        }
        else if (context.engine.getKeyState(Key::ArrowUp) == Action::Pressed && m_direction != Direction::Down) {
            m_direction = Direction::Up;
        }
        else if (context.engine.getKeyState(Key::ArrowLeft) == Action::Pressed && m_direction != Direction::Right) {
            m_direction = Direction::Left;
        }
    }
    else {
        m_direction = m_head->m_direction;
    }
}

void Snake::updatePosition(GameContext &context) {
    Sprite sprite = context.engine.getSprite(m_id);
    auto [xPos, yPos] = sprite.getPosition();
    switch (m_direction) {
        case Up: {
            sprite.setPosition(xPos, yPos + g_tileHeight);
            break;
        }
        case Left: {
            sprite.setPosition(xPos - g_tileWidth , yPos);
            break;
        }
        case Right: {
            sprite.setPosition(xPos + g_tileWidth, yPos);
            break;
        }
        case Down: {
            sprite.setPosition(xPos, yPos - g_tileHeight);
            break;
        }
    }
}

GameEvent Snake::resolveCollisions(GameContext &context) {
    return GameEvent::None;
}

GameEvent Snake::update(GameContext &context) {
    updateDirection(context);
    updatePosition(context);
    return resolveCollisions(context);
}
