#pragma once

#include <array>
#include "RaycasterConstants.h"

namespace game
{
	using GameMap = std::array<int, MAP_SZ>;

	int toMapIndex(float x, float y);
	int toMapIndex(int x, int y);
	bool isWall(float x, float y, const GameMap* map);
	bool isWall(int x, int y, const GameMap* map);
	bool isInWall(const SDL_Rect* playerVolume, const GameMap* map);
}