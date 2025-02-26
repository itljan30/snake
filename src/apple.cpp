#include "apple.h"
#include "snake.h"
#include "constants.h"

#include <RedGir/sprite.h>
#include <RedGir/color.h>

#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <cstdint>

static const Color appleColor = Color(255, 16, 0, 255);
static const int32_t appleLayer = 3;

Apple::Apple(Coords coords, SpriteId id) : m_coords(coords), m_id(id) {}

Apple::~Apple() {}

Apple Apple::dummyApple() {
    return Apple(Coords { INT32_MAX, INT32_MAX }, SpriteId { INT32_MAX });
}

Coords Apple::getCoords() {
    return m_coords;
}

Apple Apple::createNewApple(GameContext &context) {
    // This alg will work in about O(nlogn). Since the total number of tiles is relatively small,
    // this should be worth it compared to brute forcing a random number when only one tile is not occupied by snake.
    std::vector<int32_t> snakePos;

    // Reserve the total number of tiles to avoid reallocating memory.
    snakePos.reserve(g_tilesPerRow * g_tilesPerCol);
    for (Snake &snake : context.snakeParts) {
        Coords coords = snake.getCoords();
        int32_t pos = coords.y * g_tilesPerRow + coords.x;
        snakePos.push_back(pos);
    }

    // Entire grid is all snakes, no open spots left over.
    if (snakePos.size() == g_tilesPerRow * g_tilesPerCol) {
        return dummyApple();
    }

    std::sort(snakePos.begin(), snakePos.end());

    // Get random number between 0 and g_tilesPerRow * g_tilesPerCol - snakePos.size() inclusive.
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, g_tilesPerRow * g_tilesPerCol - snakePos.size());

    int32_t randTile = dist(rng);
    // Find the nth (randTile) free tile.
    for (size_t i = 0; i < snakePos.size(); i++) {
        if (snakePos.at(i) <= randTile) {
            randTile++;
        }
        else {
            break;
        }
    }

    int32_t xPos = randTile % g_tilesPerRow;
    int32_t yPos = randTile / g_tilesPerRow;

    SpriteId id = context.engine.addQuad(
        appleColor,
        xPos * g_tileSize,
        yPos * g_tileSize,
        appleLayer,
        g_tileSize,
        g_tileSize,
        context.engine.defaultShader()
    );

    return Apple(Coords { xPos, yPos }, id);
}

void Apple::removeFromEngine(GameContext &context) {
    context.engine.removeSprite(m_id);
}

GameEvent Apple::update(GameContext &context) {
    return GameEvent::None;
}
