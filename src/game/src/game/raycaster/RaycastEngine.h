#pragma once

#include "Globals.h"
#include <array>
#include "Map.h"

namespace game
{
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

	struct RenderableTexturePixel
	{
		Color color;
		SDL_Rect rect;
	};

	struct ColumnRenderData
	{
		SDL_Rect rect;
		Color color;
		Line ray;
		RenderableTexturePixel verticalPixelArray[16]; // TODO: move texture height magic number somewhere sensible
	};

	namespace raycast_engine
	{
		int toMapIndex(float x, float y);
		
		ColumnRenderData doRayTest(float x, float y, float rayAngle, float playerAngle, unsigned char facing, int pxCol, int pxWidth, std::array<int, game::MAP_SZ>* map);
	};
}