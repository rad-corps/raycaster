#include "RaycastEngine.h"
#include <iostream>
#include <algorithm>
#include "RaycasterConstants.h"
#include <cmath> // std::fmod
#include <limits> // std::numeric_limits

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

	const std::vector<ColumnRenderData>& RaycastEngine::generateWallRenderData(math::Transform playerTransform, GameMap* map, rcgf::Texture* wallTexture)
	{
		// render the 3D world from the player perspective

		constexpr float FOV_OFFSET = -(FOV / 2);

		int screenColumnNumber = 0;

		for (int column = 0; column < COLUMNS; ++column)
		{
			// px is what % across screen?
			const float pxPerc = screenColumnNumber / (float)SCREEN_WIDTH;

			// calculate the angle to raycast based on the x screen position
			const float angle = pxPerc * FOV + FOV_OFFSET;

			// sum with the player's current angle to get the angle to send to the engine
			const float rayAngle = math::sum_angle(playerTransform.angle, angle);

			// cast the ray to calculate wall height for this column
			wallData[screenColumnNumber] = doRayTest(playerTransform, rayAngle, screenColumnNumber, map, wallTexture);

#ifdef RENDER_FLOORS
			raycast_engine::drawFloorColumn(m_renderer, m_impl->player.transform, crd, screenColumnNumber, rayAngle, &m_impl->wallTexture);
#endif
			
			// which x position pixel (column number in pixels)?
			screenColumnNumber += X_PX_STEP;
		}

		return wallData;
		
	}

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
	ColumnRenderData RaycastEngine::doRayTest(const math::Transform& transform, float rayAngle, int pxCol, GameMap* map, rcgf::Texture* wallTexture)
	{
		const float& x = transform.pos.x;
		const float& y = transform.pos.y;
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

		ret.columnTexture = wallTexture;
		ret.srcRect = SDL_Rect{ (int)positionAlongWall,0,1,WALL_TEXTURE_SZ };
		ret.ray.start.x = (int)x;
		ret.ray.start.y = (int)y;
		ret.ray.end.x = (int)xIntersect;
		ret.ray.end.y = (int)yIntersect;

		// fisheye correction https://permadi.com/1996/05/ray-casting-tutorial-8/
		{
			const float angleDifference = rayAngle - playerAngle;
			distance = distance * cos(angleDifference);
		}

		ret.dstRect.x = pxCol;
		ret.dstRect.w = X_PX_STEP;

		// wall height https://permadi.com/1996/05/ray-casting-tutorial-9/
		ret.dstRect.h = static_cast<int>(MAP_CELL_PX / distance * DIST_PROJECTION_PLANE);
		ret.dstRect.y = (SCREEN_HEIGHT - ret.dstRect.h) / 2;

		return ret;
	}

	void RaycastEngine::drawFloorColumn(SDL_Renderer* renderer, const math::Transform& playerTransform, const ColumnRenderData& crd, int screenColumnNumber, float rayAngle, rcgf::Texture* tx)
	{
		// draw floors https://github.com/permadi-com/ray-cast/blob/master/demo/4/sample4.js
		// 1. start from the bottom of the wall
		for (int y = crd.dstRect.y + crd.dstRect.h; y < SCREEN_HEIGHT; ++y)
		{
			// straight distance from players feet to ground point          PLAYER_HEIGHT          
			// --------------------------------------------------- = ------------------------------
			//               DIST_PROJECTION_PLANE                     screen y pixel - CENTER_Y   

			// to find the straight distance: multiply both sides by the DIST_PROJECTION_PLANE



			const int px = y - CENTER_Y;
			const float straightDistance = DIST_PROJECTION_PLANE * (PLAYER_HEIGHT / (float)px);
			const float diagonalDistance = straightDistance * (1 / cos(rayAngle - playerTransform.angle));
			const float xEnd = diagonalDistance * cos(rayAngle) + playerTransform.pos.x;
			const float yEnd = diagonalDistance * sin(rayAngle) + playerTransform.pos.y;

			// get the texture coordinate from worldPos
			float txCoordX = std::fmodf(xEnd, (float)MAP_CELL_PX) * (WALL_TEXTURE_SZ / MAP_CELL_PX);
			float txCoordY = std::fmodf(yEnd, (float)MAP_CELL_PX) * (WALL_TEXTURE_SZ / MAP_CELL_PX);

			const Color color = tx->getPixelColor((int)txCoordX, (int)txCoordY);
			SDL_Rect dstRect{ screenColumnNumber,y,X_PX_STEP,X_PX_STEP };
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(renderer, &dstRect);
		}
	}
	
}


