#include "RenderEngine.h"

namespace game
{
	void RenderEngine::RenderWalls(const std::vector<ColumnRenderData>& crdVec)
	{
		for (const ColumnRenderData& crd : crdVec)
		{
			crd.columnTexture->render2(&crd.srcRect, &crd.dstRect);
		}
	}

	void RenderEngine::RenderTopDownMap(const game::GameMap& map, const std::vector<ColumnRenderData>& crdVec, const math::Transform& pov, const math::Transform& refTransform, bool showRays)
	{
		// draw the map
		SDL_SetRenderDrawColor(m_renderer, 0, 128, 0, 0xFF);
		for (int row = 0; row < MAP_ROWS; ++row)
		{
			for (int col = 0; col < MAP_COLS; ++col)
			{
				// calc rect position and dimension
				SDL_Rect dstRect{ col * MAP_CELL_PX * TOP_DOWN_SCALE, row * MAP_CELL_PX * TOP_DOWN_SCALE, MAP_CELL_PX * TOP_DOWN_SCALE, MAP_CELL_PX * TOP_DOWN_SCALE };
				if (map[col + row * MAP_COLS] > 0)
				{
					SDL_RenderFillRect(m_renderer, &dstRect);
				}
				else
				{
					SDL_RenderDrawRect(m_renderer, &dstRect);
				}
			}
		}

		// draw rays from player
		// hack, draw the ray line here since we stopped caching the ColumnRenderData				
		for (const auto& crd : crdVec)
		{
			if (showRays)
			{
				SDL_SetRenderDrawColor(m_renderer, 133, 133, 173, 255);
				SDL_RenderDrawLine(m_renderer, crd.ray.start.x * TOP_DOWN_SCALE, crd.ray.start.y * TOP_DOWN_SCALE, crd.ray.end.x * TOP_DOWN_SCALE, crd.ray.end.y * TOP_DOWN_SCALE);
			}
		}

		// draw line from player to enemy
		SDL_RenderDrawLine(m_renderer, (int)pov.pos.x * TOP_DOWN_SCALE, (int)pov.pos.y * TOP_DOWN_SCALE, (int)refTransform.pos.x * TOP_DOWN_SCALE, (int)refTransform.pos.y * TOP_DOWN_SCALE);
		const math::Vec2 povForwardVec = math::angle_to_vec(pov.angle);
		math::Vec2 scaledPlayerDir = math::scale(povForwardVec, 100.f);
		SDL_SetRenderDrawColor(m_renderer, 255, 100, 0, 0xFF);
		// draw player forward vector
		SDL_RenderDrawLine(m_renderer, (int)pov.pos.x * TOP_DOWN_SCALE, (int)pov.pos.y * TOP_DOWN_SCALE, (int)(pov.pos.x + scaledPlayerDir.x) * TOP_DOWN_SCALE, (int)(pov.pos.y + scaledPlayerDir.y) * TOP_DOWN_SCALE);
		

		//m_impl->player.render(m_renderer);		
	}

	void RenderEngine::RenderSprite(const math::Transform& povTransform, const Sprite& sprite) const
	{
		// find angle from player to enemy
		// 1. create two vectors
		//  1.1 one for the forward direction of the player
		const math::Vec2& playerPos = povTransform.pos;
		const math::Vec2& spritePos = sprite.m_transform.pos;
		const math::Vec2 playerForward = math::angle_to_vec(povTransform.angle);

		//  1.2 one for the direction from player to enemy
		const math::Vec2 playerToSprite = spritePos - playerPos;
		const float playerForwardToSpriteAngle = math::angle(playerForward, playerToSprite);
		float frustumToSpriteAngle = -1.f;

		// if cross is < 0 then enemy is RHS of screen, else LHS of screen
		if (playerForwardToSpriteAngle < FOV / 2)
		{
			if (math::cross(math::normalize(playerToSprite), math::normalize(playerForward)) < 0)
			{
				// RHS of screen
				// add half of FOV to angle
				frustumToSpriteAngle = playerForwardToSpriteAngle + FOV / 2;
			}
			else
			{
				// LHS of screen
				// angleToEnemy starts as the player forward vector, so lets subtr
				frustumToSpriteAngle = FOV / 2 - playerForwardToSpriteAngle;
			}

			// now we have an angle that is 0 at far left of FOV, and FOV at far right.
			// convert angle to screen space
			const float screenX = (frustumToSpriteAngle / FOV) * SCREEN_WIDTH;

			// calculate the screenY position
			// ================================
			// playerToEnemy.length()          PLAYER_HEIGHT
			// ----------------------  =   ---------------------
			// DIST_PROJECTION_PLANE         screenY - CENTER_Y
			const float distanceToSprite = math::magnitude(playerToSprite);

			const float screenYBottomOfSprite = ((DIST_PROJECTION_PLANE * PLAYER_HEIGHT) / distanceToSprite) + CENTER_Y;

			//find the height of the sprite
			const int spriteHeight = static_cast<int>(MAP_CELL_PX / distanceToSprite * DIST_PROJECTION_PLANE);

			// TODO: calculate this based on angle between player and sprite
			const int animID = 0;
			SDL_Rect dstRect;
			dstRect.h = spriteHeight;
			dstRect.w = spriteHeight;
			dstRect.x = (int)screenX - spriteHeight / 2;
			dstRect.y = (int)screenYBottomOfSprite - spriteHeight;
			sprite.m_spritesheet->render(animID, &dstRect);
		}
	}
}

