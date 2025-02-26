#include "snake.h"
#include "apple.h"
#include "utils.h"
#include "constants.h"

#include <RedGir/engine.h>

#include <cstdint>

const Color snakeColor = Color(0, 64, 255, 255);
constexpr uint32_t snakeLayer = 5;

void Snake::addNewSnakeToSnakeParts(GameContext &context) {
    if (context.snakeParts.size() == 0) {
        int32_t xPos = g_tilesPerRow / 2;
        int32_t yPos = g_tilesPerCol / 2;

        SpriteId snakeId = context.engine.addQuad(
            snakeColor,
            xPos * g_tileSize,
            yPos * g_tileSize,
            snakeLayer,
            g_tileSize,
            g_tileSize,
            context.engine.defaultShader() // TODO Use CRT shader instead
        );

        context.snakeParts.emplace_back(
            snakeId,
            Coords {
                xPos,
                yPos
            },
            nullptr,
            Direction::None
        );
    }
    else {
        Snake &lastPart = context.snakeParts.back();
        SpriteId newPartId = context.engine.addQuad(
            snakeColor,
            lastPart.m_coords.x * g_tileSize,
            lastPart.m_coords.y * g_tileSize,
            snakeLayer,
            g_tileSize,
            g_tileSize,
            context.engine.defaultShader() // TODO Use CRT shader instead
        );

        context.snakeParts.emplace_back(
            newPartId,
            Coords {
                lastPart.m_coords.x,
                lastPart.m_coords.y
            },
            &lastPart,
            lastPart.m_direction
        );
    }
} 

Snake::Snake(SpriteId id, Coords coords, Snake *head, Direction direction)
    : m_id(id), m_head(head), m_coords(coords), m_direction(direction) {}

Snake::~Snake() {}

void Snake::updateDirection(GameContext &context) {
    if (m_head != nullptr) {
        m_direction = m_head->m_direction;
        return;
    }

    if ((context.engine.getKeyState(Key::ArrowDown) == Action::Pressed || 
        context.engine.getKeyState(Key::ArrowDown) == Action::Held) && 
        m_direction != Direction::Up 
    ) {
        m_direction = Direction::Down;
    }

    else if ((context.engine.getKeyState(Key::ArrowRight) == Action::Pressed ||
        context.engine.getKeyState(Key::ArrowRight) == Action::Held) && 
        m_direction != Direction::Left 
    ) {
        m_direction = Direction::Right;
    }

    else if ((context.engine.getKeyState(Key::ArrowUp) == Action::Pressed ||
        context.engine.getKeyState(Key::ArrowUp) == Action::Held) && 
        m_direction != Direction::Down 
    ) {
        m_direction = Direction::Up;
    }

    else if ((context.engine.getKeyState(Key::ArrowLeft) == Action::Pressed ||
        context.engine.getKeyState(Key::ArrowLeft) == Action::Held) && 
        m_direction != Direction::Right 
    ) {
        m_direction = Direction::Left;
    }
}

Coords Snake::getCoords() {
    return m_coords;
}

GameEvent Snake::updatePosition(GameContext &context) {
    Sprite sprite = context.engine.getSprite(m_id);
    auto [xPos, yPos] = m_coords;
    switch (m_direction) {
        case Direction::None: break;
        case Direction::Up: {
            m_coords = Coords { xPos, yPos + 1 };
            break;
        }
        case Direction::Left: {
            m_coords = Coords { xPos - 1, yPos };
            break;
        }
        case Direction::Right: {
            m_coords = Coords { xPos + 1, yPos };
            break;
        }
        case Direction::Down: {
            m_coords = Coords { xPos, yPos - 1 };
            break;
        }
    }

    if (m_coords.x < 0 || m_coords.x > g_screenWidth / g_tileSize - 1 || 
        m_coords.y < 0 || m_coords.y > g_screenHeight / g_tileSize - 1) {
        return GameEvent::GameOver;
    }

    sprite.setPosition(m_coords.x * g_tileSize, m_coords.y * g_tileSize);
    return GameEvent::None;
}

GameEvent Snake::resolveCollisions(GameContext &context) {
    // Only checking collisions for the head of the snake.
    if (m_head != nullptr) {
        return GameEvent::None;
    }

    Coords appleCoords = context.apple.getCoords();
    if (m_coords == appleCoords) {
        return GameEvent::AppleConsumed;
    }

    for (Snake &other : context.snakeParts) {
        if (*this == other) {
            continue;
        }
        
        if (m_coords == other.m_coords) {
            return GameEvent::GameOver;
        }
    }

    return GameEvent::None;
}

GameEvent Snake::update(GameContext &context) {
    updateDirection(context);
    GameEvent res = updatePosition(context);
    if (res != GameEvent::None) {
        return res;
    }
    return resolveCollisions(context);
}
