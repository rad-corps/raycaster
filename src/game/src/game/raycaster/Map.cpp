#include "Map.h"
#include <cassert>

namespace game
{


	int toMapIndex(float x, float y)
	{
		const int xIndex = ((int)x / MAP_CELL_PX);
		const int yIndex = ((int)y / MAP_CELL_PX);
		const int mapIndex = xIndex + yIndex * MAP_COLS;

		// its possible to calculate a bad value with bad input, so lets
		// just agree to return -1
		if (mapIndex < 0 || MAP_SZ <= mapIndex)
		{
			return -1;
		}
		return mapIndex;
	}

	bool isWall(float x, float y, GameMap* map)
	{
		const int mapIndex = toMapIndex(x, y);

		// deal with -1 return value
		if (mapIndex == -1)
			return true;

		assert(0 <= mapIndex && mapIndex < MAP_SZ);
		return 0 < (*map)[mapIndex];
	}
}