#include "Map.h"
#include <cassert>
#include "RCGFMath.h"

using math::PI;

namespace
{
	// todo: we need to be able to replace the map
	const game::map::GameMap globalMap =
	{
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,
		1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,
		1,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1,
		1,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1,
		1,0,0,0,0,1,0,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,1,1,0,1,
		1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};
}

namespace game::map
{
	const GameMap& get_map()
	{
		return globalMap;
	}
	bool facingDown(float angle_)
	{
		return 0 < angle_ && angle_ < PI;
	}
	bool facingUp(float angle_)
	{
		return PI < angle_;
	}
	bool facingRight(float angle_)
	{
		return angle_ < PI / 2 || 3 * PI / 2 < angle_;
	}
	bool facingLeft(float angle_)
	{
		return PI / 2 < angle_ && angle_ < 3 * PI / 2;
	}

	unsigned char getFacing(float angle_)
	{
		unsigned char ret = 0;
		if (facingUp(angle_)) ret |= Facing::UP;
		if (facingRight(angle_)) ret |= Facing::RIGHT;
		if (facingDown(angle_)) ret |= Facing::DOWN;
		if (facingLeft(angle_)) ret |= Facing::LEFT;
		return ret;
	}

	math::Vec2 to_world_position(int mapIndex)
	{
		int yIndex = mapIndex / MAP_COLS;
		int xIndex = mapIndex % MAP_COLS;
		return math::Vec2{ static_cast<float>(xIndex * MAP_CELL_PX), static_cast<float>(yIndex * MAP_CELL_PX) };
	}

	int to_map_index(int x, int y)
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

	int to_map_index(const math::Vec2& pos)
	{
		return to_map_index(static_cast<int>(pos.x), static_cast<int>(pos.y));
	}

	std::array<int, 8> get_adjacent_map_indices(int idx)
	{
		/**
		*  [0][1][2]
		*  [3]idx[4]
		*  [5][6][7]
		*/

		//                       0   1   2   3   4   5   6   7
		std::array<int, 8> ret{ -1, -1, -1, -1, -1, -1, -1, -1 };

		// [0][1][2]
		if (idx >= MAP_COLS) // not the top row
		{
			if (idx % MAP_COLS != 0) // not the left most column
			{
				ret[0] = idx - MAP_COLS - 1;
			}
			
			ret[1] = idx - MAP_COLS;

			if ((idx + 1) % MAP_COLS != 0) // not the right most column
			{
				ret[2] = idx - MAP_COLS + 1;
			}
		}

		// [3]idx[4]
		if (idx % MAP_COLS != 0) // not the left most column
		{
			ret[3] = idx - 1;
		}
		
		if ((idx + 1) % MAP_COLS != 0) // not the right most column
		{
			ret[4] = idx + 1;
		}

		// [5][6][7]
		if (idx + MAP_COLS < MAP_SZ)
		{
			if (idx % MAP_COLS != 0) // not the left most column
			{
				ret[5] = idx + MAP_COLS - 1;
			}
			
			ret[6] = idx + MAP_COLS;
			
			if ((idx + 1) % MAP_COLS != 0) // not the right most column
			{
				ret[7] = idx + MAP_COLS + 1;
			}
		}
		return ret;
	}

	// TODO: Remove?
	bool is_wall(float x, float y)
	{
		return is_wall(static_cast<int>(x), static_cast<int>(y));
	}

	bool is_wall(int x, int y)
	{
		const int mapIndex = to_map_index(x, y);

		// deal with -1 return value
		if (mapIndex == -1)
			return true;

		assert(0 <= mapIndex && mapIndex < MAP_SZ);
		return globalMap[mapIndex] > 0;
	}

	bool is_wall(int index)
	{
		assert(0 <= index && index < MAP_SZ);
		return globalMap[index] > 0;
	}



	bool is_in_wall(const SDL_Rect* playerVolume)
	{
		// player volume must be smaller than map cell size for this optimization to work
		assert(playerVolume->w <= MAP_CELL_PX && playerVolume->h <= MAP_CELL_PX);
		
		// check the 4 corners of the rect? 
		if (is_wall(playerVolume->x, playerVolume->y))                                     return true; // TL
		if (is_wall(playerVolume->x + playerVolume->w, playerVolume->y))                   return true; // TR
		if (is_wall(playerVolume->x + playerVolume->w, playerVolume->y + playerVolume->h)) return true; // BR
		if (is_wall(playerVolume->x, playerVolume->y + playerVolume->h))                   return true; // BL

		return false;
	}

	RayWallCollision find_wall_hit_pos(const math::Vec2& pos, float rayAngle)
	{
		RayWallCollision ret{};

		const float& x = pos.x;
		const float& y = pos.y;
		//const float& playerAngle = transform.angle;
		const unsigned char facing = getFacing(rayAngle);

		float rowIntersectDistance = 10000000.f;
		float colIntersectDistance = 10000000.f;

		// check horizontals
		// TODO: collapse LEFT and RIGHT branches together
		if (facing & Facing::RIGHT)
		{
			const int firstColIntersect = ((int)x / MAP_CELL_PX) * MAP_CELL_PX + MAP_CELL_PX;
			const float tempAngle = rayAngle;
			const float adjLen = firstColIntersect - x;
			const float oppLen = tan(tempAngle) * adjLen;
			const float xOffset = MAP_CELL_PX;
			const float yOffset = tan(tempAngle) * MAP_CELL_PX;
			float checkX = (float)firstColIntersect;
			float checkY = oppLen + y;

			while (!is_wall(checkX, checkY))
			{
				checkX += xOffset; checkY += yOffset;
			}
			colIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
			ret.distance = colIntersectDistance;
			ret.xHitPos = checkX;
			ret.yHitPos = checkY;

			ret.positionAlongWall = std::fmodf(ret.yHitPos, (float)MAP_CELL_PX);
			ret.positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
			ret.wallFaceDir = Facing::RIGHT;
		}
		else if (facing & Facing::LEFT)
		{
			const int firstColIntersect = ((int)x / MAP_CELL_PX) * MAP_CELL_PX;
			const float tempAngle = (2 * PI - rayAngle);
			const float adjLen = x - firstColIntersect;
			const float oppLen = tan(tempAngle) * adjLen;
			const float xOffset = -MAP_CELL_PX;
			const float yOffset = tan(tempAngle) * MAP_CELL_PX;
			float checkX = (float)firstColIntersect - 0.1f;
			float checkY = oppLen + y;

			while (!is_wall(checkX, checkY))
			{
				checkX += xOffset; checkY += yOffset;
			}
			colIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
			ret.distance = colIntersectDistance;
			ret.xHitPos = checkX;
			ret.yHitPos = checkY;

			ret.positionAlongWall = std::fmodf(ret.yHitPos, (float)MAP_CELL_PX);
			ret.positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
			ret.wallFaceDir = Facing::LEFT;
		}
		// check verticals
		// TODO: collapse UP and DOWN branches together
		if (facing & Facing::UP)
		{
			const int firstRowIntersect = ((int)y / MAP_CELL_PX) * MAP_CELL_PX;
			const float tempAngle = rayAngle - 3 * PI / 2;
			const float adjLen = y - firstRowIntersect;
			const float oppLen = tan(tempAngle) * adjLen;
			const float xOffset = tan(tempAngle) * MAP_CELL_PX;
			const float yOffset = -MAP_CELL_PX;
			float checkX = oppLen + x;
			float checkY = (float)firstRowIntersect - 0.1f;
			while (!is_wall(checkX, checkY))
			{
				checkX += xOffset; checkY += yOffset;
			}
			rowIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
			if (rowIntersectDistance < colIntersectDistance)
			{
				ret.distance = rowIntersectDistance;
				ret.xHitPos = checkX;
				ret.yHitPos = checkY;

				ret.positionAlongWall = std::fmodf(ret.xHitPos, (float)MAP_CELL_PX);
				ret.positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
				ret.wallFaceDir = Facing::UP;
			}
		}
		else if (facing & Facing::DOWN)
		{
			const int firstRowIntersect = ((int)y / MAP_CELL_PX) * MAP_CELL_PX + MAP_CELL_PX;
			const float tempAngle = PI / 2 - rayAngle;
			const float adjLen = firstRowIntersect - y;
			const float oppLen = tan(tempAngle) * adjLen;
			const float xOffset = tan(tempAngle) * MAP_CELL_PX;
			const float yOffset = MAP_CELL_PX;
			float checkX = oppLen + x;
			float checkY = (float)firstRowIntersect;
			while (!is_wall(checkX, checkY))
			{
				checkX += xOffset; checkY += yOffset;
			}
			rowIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
			if (rowIntersectDistance < colIntersectDistance)
			{
				ret.distance = rowIntersectDistance;
				ret.xHitPos = checkX;
				ret.yHitPos = checkY;

				ret.positionAlongWall = std::fmodf(ret.xHitPos, (float)MAP_CELL_PX);
				ret.positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
				ret.wallFaceDir = Facing::DOWN;
			}
		}

		// TODO: move fisheye correction to the callsite for wall raycasting. 
		//       we do not want this correction when measuring distance from enemy to player
		// fisheye correction https://permadi.com/1996/05/ray-casting-tutorial-8/
		//const float angleDifference = rayAngle - playerAngle;
		//ret.distance *= cos(angleDifference);

		return ret;
	}
}