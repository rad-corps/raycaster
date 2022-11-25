#include "Map.h"
#include <cassert>

namespace game
{

	// TODO: Remove? 
	int toMapIndex(float x, float y)
	{
		return toMapIndex(static_cast<int>(x), static_cast<int>(y));
	}

	int toMapIndex(int x, int y)
	{
		const int xIndex = x / MAP_CELL_PX;
		const int yIndex = y / MAP_CELL_PX;
		const int mapIndex = xIndex + yIndex * MAP_COLS;

		// its possible to calculate a bad value with bad input, so lets
		// just agree to return -1
		if (mapIndex < 0 || MAP_SZ <= mapIndex)
		{
			//assert(false);
			return -1;
		}
		return mapIndex;
	}

	// TODO: Remove?
	bool isWall(float x, float y, const GameMap* map)
	{
		return isWall(static_cast<int>(x), static_cast<int>(y), map);
	}

	bool isWall(int x, int y, const GameMap* map)
	{
		const int mapIndex = toMapIndex(x, y);

		// deal with -1 return value
		if (mapIndex == -1)
			return true;

		assert(0 <= mapIndex && mapIndex < MAP_SZ);
		return 0 < (*map)[mapIndex];
	}

	bool isInWall(const SDL_Rect* playerVolume, const GameMap* map)
	{
		// player volume must be smaller than map cell size for this optimization to work
		assert(playerVolume->w <= MAP_CELL_PX && playerVolume->h <= MAP_CELL_PX);
		
		// check the 4 corners of the rect? 
		if (isWall(playerVolume->x, playerVolume->y, map))                                     return true; // TL
		if (isWall(playerVolume->x + playerVolume->w, playerVolume->y, map))                   return true; // TR
		if (isWall(playerVolume->x + playerVolume->w, playerVolume->y + playerVolume->h, map)) return true; // BR
		if (isWall(playerVolume->x, playerVolume->y + playerVolume->h, map))                   return true; // BL

		return false;
	}
}