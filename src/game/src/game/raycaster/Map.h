#pragma once

#include <array>
#include "RaycasterConstants.h"

namespace game
{
	using GameMap = std::array<int, MAP_SZ>;

	int toMapIndex(float x, float y);
	bool isWall(float x, float y, GameMap* map);
}