#include "RaycastEngine.h"
#include <iostream>
#include <algorithm>
#include "RaycasterConstants.h"
#include <cmath> // std::fmod
#include <limits> // std::numeric_limits

using math::PI;

namespace game
{
	const std::vector<ColumnRenderData>& RaycastEngine::generateWallRenderData(math::Transform playerTransform, GameMap* map, rcgf::Texture* wallTexture)
	{
		// render the 3D world from the player perspective

		constexpr float FOV_OFFSET = -(FOV / 2);

		int screenColumnNumber = 0;

		for (int column = 0; column < COLUMNS; ++column)
		{
			// px is what % across screen?
			const float pxPerc = (float)screenColumnNumber / (float)SCREEN_WIDTH;

			// calculate the angle to raycast based on the x screen position
			const float angle = pxPerc * FOV + FOV_OFFSET;

			// sum with the player's current angle to get the angle to send to the engine
			const float rayAngle = math::sum_angle(playerTransform.angle, angle);

			// cast the ray to calculate wall height for this column
			wallData[screenColumnNumber] = DoRayTest(playerTransform, rayAngle, screenColumnNumber, map, wallTexture);

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
	ColumnRenderData RaycastEngine::DoRayTest(const math::Transform& transform, float rayAngle, int pxCol, GameMap* map, rcgf::Texture* wallTexture)
	{
		RayWallCollision rayWallCollision = FindWallHitPos(transform, rayAngle, map);

		ColumnRenderData ret;
		ret.distance = rayWallCollision.distance;
		ret.columnTexture = wallTexture;
		ret.srcRect = SDL_Rect{ (int)rayWallCollision.positionAlongWall,0,1,WALL_TEXTURE_SZ };
		ret.ray.start.x = transform.pos.x;
		ret.ray.start.y = transform.pos.y;
		ret.ray.end.x = rayWallCollision.xHitPos;
		ret.ray.end.y = rayWallCollision.yHitPos;
		ret.dstRect.x = pxCol;
		ret.dstRect.w = X_PX_STEP;

		// wall height https://permadi.com/1996/05/ray-casting-tutorial-9/
		ret.dstRect.h = static_cast<int>(MAP_CELL_PX / rayWallCollision.distance * DIST_PROJECTION_PLANE);
		ret.dstRect.y = (SCREEN_HEIGHT - ret.dstRect.h) / 2;

		return ret;
	}

	void RaycastEngine::DrawFloorColumn(SDL_Renderer* renderer, const math::Transform& playerTransform, const ColumnRenderData& crd, int screenColumnNumber, float rayAngle, rcgf::Texture* tx)
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


