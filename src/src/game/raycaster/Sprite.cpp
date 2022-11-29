#include "Sprite.h"
#include "RaycasterConstants.h"

namespace game
{
	// TODO: move this to RenderEngine
	void Sprite::render(const math::Transform& povTransform) const
	{
		// find angle from player to enemy
		// 1. create two vectors
		//  1.1 one for the forward direction of the player
		const math::Vec2& playerPos = povTransform.pos;
		const math::Vec2& spritePos = m_transform.pos;
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
			m_spritesheet->render(animID, &dstRect);
		}
	}
}