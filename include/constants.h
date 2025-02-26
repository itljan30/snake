#pragma once

#include <cstdint>

constexpr uint32_t g_screenWidth = 800;
constexpr uint32_t g_screenHeight = 600;

constexpr uint32_t g_tileSize = 20;

constexpr uint32_t g_tilesPerRow = g_screenWidth / g_tileSize;
constexpr uint32_t g_tilesPerCol = g_screenHeight / g_tileSize;

constexpr float g_targetFps = 60.0;
