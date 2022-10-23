#pragma once

#include "RaycasterConstants.h"
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

	struct ColumnRenderData
	{
		int wallMapIndex;
		int textureXPos;
		SDL_Rect rect;
		Color color;
		Line ray;
	};

	namespace raycast_engine
	{
		int toMapIndex(float x, float y);
		
		ColumnRenderData doRayTest(const Transform& transform, float rayAngle, int pxCol, GameMap* map);
	};
}