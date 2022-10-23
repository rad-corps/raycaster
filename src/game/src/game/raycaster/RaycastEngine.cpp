#include "RaycastEngine.h"
#include <iostream>
#include <algorithm>
#include "RaycasterConstants.h"
#include <cmath> // std::fmod
#include <limits> // std::numeric_limits

namespace game
{
	bool operator==(const WallMapFace& lhs, const WallMapFace& rhs)
	{
		return
			lhs.mapIndex == rhs.mapIndex
			&& lhs.facing == rhs.facing;
	}

	bool operator!=(const WallMapFace& lhs, const WallMapFace& rhs)
	{
		return !(lhs == rhs);
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

	namespace raycast_engine
	{
		/// <summary>
		/// 
		/// </summary>
		/// <param name="x">player x position</param>
		/// <param name="y">player y position</param>
		/// <param name="rayAngle">individual ray angle (0 - 2*PI)</param>
		/// <param name="playerAngle">player angle (0 - 2*PI)</param>
		/// <param name="pxCol"></param>
		/// <param name="pxWidth"></param>
		/// <param name="map"></param>
		/// <returns></returns>
		ColumnRenderData doRayTest(const Transform& transform, float rayAngle, int pxCol, GameMap* map)
		{
			const float& x = transform.x;
			const float& y = transform.y;
			const float& playerAngle = transform.angle;

			unsigned char facing = getFacing(rayAngle);
		
			float rowIntersectDistance = 10000000.f;
			float colIntersectDistance = 10000000.f;
			float distance = 10000000.f;
			float xIntersect = 10000000.f;
			float yIntersect = 10000000.f;
			float positionAlongWall = 0; // between 0-1
			char faceDirection = std::numeric_limits<char>::max();

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
				distance = colIntersectDistance;
				xIntersect = checkX;
				yIntersect = checkY;
				
				positionAlongWall = std::fmodf(yIntersect, (float)MAP_CELL_PX);
				positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
				faceDirection = Facing::RIGHT;
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
				distance = colIntersectDistance;
				xIntersect = checkX;
				yIntersect = checkY;

				positionAlongWall = std::fmodf(yIntersect, (float)MAP_CELL_PX);
				positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
				faceDirection = Facing::LEFT;
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
					distance = rowIntersectDistance;
					xIntersect = checkX;
					yIntersect = checkY;

					positionAlongWall = std::fmodf(xIntersect, (float)MAP_CELL_PX);
					positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
					faceDirection = Facing::UP;
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
					distance = rowIntersectDistance;
					xIntersect = checkX;
					yIntersect = checkY;

					positionAlongWall = std::fmodf(xIntersect, (float)MAP_CELL_PX);
					positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
					faceDirection = Facing::DOWN;
				}
			}

			ColumnRenderData ret;

			ret.wallMapFace.mapIndex = game::toMapIndex(xIntersect, yIntersect);
			ret.wallMapFace.facing = faceDirection;
			ret.textureXPos = (int)positionAlongWall;
			ret.ray.start.x = (int)x;
			ret.ray.start.y = (int)y;
			ret.ray.end.x = (int)xIntersect;
			ret.ray.end.y = (int)yIntersect;

			// need this for the fisheye correction
			const float angleDifference = rayAngle - playerAngle;

			ret.rect.x = pxCol;
			ret.rect.w = X_PX_STEP;
			ret.rect.h = (int)((SCREEN_HEIGHT * 10) / (distance * cos(angleDifference)));
			ret.rect.y = (SCREEN_HEIGHT - ret.rect.h) / 2;

			if (rowIntersectDistance < colIntersectDistance)
			{
				ret.color.r = 0;
				ret.color.g = 0;
				ret.color.b = 0;
				ret.color.a = (Uint8)std::max(0, (int)rowIntersectDistance); // distance fade
			}
			else
			{
				ret.color.r = 0;
				ret.color.g = 0;
				ret.color.b = 0;
				ret.color.a = (Uint8)std::max(0, (int)colIntersectDistance); // distance fade
			}
			return ret;
		}
	}
}


