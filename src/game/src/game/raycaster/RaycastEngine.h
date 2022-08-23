#pragma once

#include "Globals.h"
#include <array>

namespace game
{
	constexpr int MAP_CELL_PX = 16;
	constexpr int MAP_COLS = 32;
	constexpr int MAP_ROWS = 32;
	constexpr int MAP_SZ = MAP_COLS * MAP_ROWS;

	enum Facing : unsigned char
	{
		UP = 0x0001,
		RIGHT = 0x0002,
		DOWN = 0x0004,
		LEFT = 0x0008,
	};

	bool facingDown(float angle_);
	bool facingUp(float angle_);
	bool facingRight(float angle_);
	bool facingLeft(float angle_);
	unsigned char getFacing(float angle_);

	struct Color
	{
		Uint8 r;
		Uint8 g;
		Uint8 b;
		Uint8 a;
	};

	struct Line
	{
		SDL_Point start;
		SDL_Point end;
	};

	struct ColumnRenderData
	{
		SDL_Rect rect;
		Color color;
		Line ray;
	};

	struct RayTest
	{
		void drawIntersect(int x, int y);
		int toMapIndex(float x, float y);
		bool isWall(float x, float y, std::array<int, game::MAP_SZ>* map);
		ColumnRenderData doRayTest(float x, float y, float rayAngle, float playerAngle, unsigned char facing, int pxCol, int pxWidth, std::array<int, game::MAP_SZ>* map);
	};
}