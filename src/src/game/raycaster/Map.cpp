#include "Map.h"
#include <cassert>
#include "RCGFMath.h"

using math::PI;

namespace game
{
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

	RayWallCollision FindWallHitPos(const math::Transform& transform, float rayAngle, const GameMap* map)
	{
		RayWallCollision ret{};

		const float& x = transform.pos.x;
		const float& y = transform.pos.y;
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

			while (!isWall(checkX, checkY, map))
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

			while (!isWall(checkX, checkY, map))
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
			while (!isWall(checkX, checkY, map))
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
			while (!isWall(checkX, checkY, map))
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