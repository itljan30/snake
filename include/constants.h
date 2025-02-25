#pragma once

#include <cstdint>
#include <cmath>

constexpr uint32_t g_screenWidth = 800;
constexpr uint32_t g_screenHeight = 600;

const uint32_t g_tileSize = 20;

const uint32_t g_tilesPerRow = g_screenWidth / g_tileSize;
const uint32_t g_tilesPerCol = g_screenHeight / g_tileSize;


constexpr float g_targetFps = 60.0;
