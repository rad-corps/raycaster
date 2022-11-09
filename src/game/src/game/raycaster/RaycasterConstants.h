#pragma once

#include "Globals.h"

namespace game
{
	constexpr int MAP_CELL_PX = 8;
	constexpr int WALL_TEXTURE_SZ = 64;
	constexpr int WALL_TEXTURE_W = WALL_TEXTURE_SZ;
	constexpr int WALL_TEXTURE_H = WALL_TEXTURE_SZ;
	constexpr int MAP_COLS = 32;
	constexpr int MAP_ROWS = 32;
	constexpr int MAP_SZ = MAP_COLS * MAP_ROWS;

	constexpr float FOV = PI / 3.f; // 60 degrees
	constexpr int X_PX_STEP = 1;
	constexpr int COLUMNS = SCREEN_WIDTH / X_PX_STEP;
	constexpr float START_ANGLE = 0.f;

	struct Transform
	{
		float x;
		float y;
		float angle;
	};

	struct Vec2
	{
		float x;
		float y;
	};

	game::Vec2 angleToVec(float angle);
}