#include "RenderEngine.h"
#include <cassert>
#include <iostream>


namespace
{

	struct TopDownLine
	{
		TopDownLine(math::Vec2 point, math::Vec2 velocity, Color color) :point{ point }, velocity{ velocity }, color{ color }{}
		math::Vec2 point;
		math::Vec2 velocity;
		Color color;
	};
	std::vector<TopDownLine> topDownLines;
	void RegisterTransitiveTopDownData(TopDownLine tdl)
	{
		topDownLines.push_back(tdl);
	}
	void ResetTransitiveTopDownData()
	{
		topDownLines.clear();
	}


	bool IsObstructed(const std::vector<game::ColumnRenderData>& crdVec, const SDL_Rect& screenSpaceSprite, float distanceToSprite)
	{
		const int lBound = std::max(screenSpaceSprite.x, 0);
		const int rBound = std::min(screenSpaceSprite.x + screenSpaceSprite.w, SCREEN_WIDTH - 1);

		assert(lBound >= 0);
		assert(rBound < SCREEN_WIDTH);
		assert(lBound <= rBound);

		for (int x = lBound; x <= rBound; ++x)
		{
			const game::ColumnRenderData& crd = crdVec[x];
			if (crd.distance < distanceToSprite)
			{
				return true;
			}
		}
		return false;
	}

	struct RectContainer
	{
		RectContainer(SDL_Rect screenSpaceRect, SDL_Rect spriteSheetRect) 
			: screenSpaceRect{ screenSpaceRect }
			, spriteSheetRect{ spriteSheetRect }
			, display{true}
		{}
		RectContainer() 
			: display{ false }
			, screenSpaceRect{ 0,0,0,0 }
			, spriteSheetRect{ 0,0,0,0 }
		{}

		SDL_Rect screenSpaceRect;
		SDL_Rect spriteSheetRect;
		bool display;
	};

	RectContainer GetWallClippedSprite(const std::vector<game::ColumnRenderData>& crdVec, const SDL_Rect& screenSpaceSprite, float distanceToSprite, int spriteDims)
	{
		const int lBound = std::max(screenSpaceSprite.x, 0);
		const int rBound = std::min(screenSpaceSprite.x + screenSpaceSprite.w, SCREEN_WIDTH - 1);

		assert(lBound >= 0);
		assert(rBound < SCREEN_WIDTH);
		assert(lBound <= rBound);

		bool lhsObstructed = crdVec[lBound].distance < distanceToSprite;
		bool rhsObstructed = crdVec[rBound].distance < distanceToSprite;

		// if both lhs and rhs are not obstructed, assume no obstruction (I think this checks out, as long as our sprite sizes are <= wallsize)
		// TODO: there may be partial obstruction that doesn't cover the edges
		if (!lhsObstructed && !rhsObstructed)
		{
			return RectContainer{ 
				screenSpaceSprite, 
				SDL_Rect{0,0,spriteDims,spriteDims}
			};
		}

		// if both sides obstructed, assume it is completely obstructed
		// TODO: it may not be completely obstructed.
		if (lhsObstructed && rhsObstructed)
		{
			return RectContainer{};
		}
			

		if (lhsObstructed)
		{
			//march forward until no obstruction then snip the screenSpaceSprite rect
			for (int x = lBound; x <= rBound; ++x)
			{
				const game::ColumnRenderData& crd = crdVec[x];
				if (crd.distance > distanceToSprite)
				{
					const int spritesheetX = int((((float)(x - lBound) / (float)screenSpaceSprite.w) * (float)spriteDims));
					// found unobstructed column. assume no obstruction for rest of sprite (lots of assumptions :D)
					return RectContainer{
						
						SDL_Rect{ // Screenspace rect
							x,
							screenSpaceSprite.y,
							screenSpaceSprite.w - (x - lBound),
							screenSpaceSprite.h
						},
						SDL_Rect{ // spritesheet rect
							spritesheetX,
							0,
							spriteDims - spritesheetX,
							spriteDims
						}
					};
				}
			}
		}
		else if (rhsObstructed)
		{
			//march backwards until no obstruction then snip the screenSpaceSprite rect
			for (int x = rBound; x >= lBound; --x)
			{
				const game::ColumnRenderData& crd = crdVec[x];
				if (crd.distance > distanceToSprite)
				{
					const float percentSpriteDisplayed = (float)(x - lBound) / (float)screenSpaceSprite.w;
					const float spritesheetWidth = percentSpriteDisplayed * (float)spriteDims;
					const float screenSpaceWidth = percentSpriteDisplayed * screenSpaceSprite.w;

					return RectContainer{

						SDL_Rect{ // Screenspace rect
							lBound,
							screenSpaceSprite.y,
							(int)screenSpaceWidth,
							screenSpaceSprite.h
						},
						SDL_Rect{ // spritesheet rect
							0,
							0,
							(int)spritesheetWidth,
							spriteDims
						}
					};
				}
			}
		}

		// TODO: rhsObstructed

		return RectContainer{ screenSpaceSprite, SDL_Rect{0,0,spriteDims,spriteDims} };
	}
}

namespace game
{
	void RenderEngine::RenderWalls()
	{
		// hacky to do this here. but we need to guaruntee it is called every frame.
		ResetTransitiveTopDownData();

		for (const ColumnRenderData& crd : crdVec)
		{
			crd.columnTexture->render2(&crd.srcRect, &crd.dstRect);
		}
	}

	void RenderEngine::RenderTopDownMap(const game::GameMap& map, const math::Transform& pov, bool showRays)
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
		for (auto& tdl : topDownLines)
		{
			SDL_SetRenderDrawColor(m_renderer, tdl.color);
			SDL_RenderDrawLineF(
			m_renderer, 
				tdl.point.x * TOP_DOWN_SCALE, 
				tdl.point.y * TOP_DOWN_SCALE, 
				(tdl.point.x + tdl.velocity.x) * TOP_DOWN_SCALE,
				(tdl.point.y + tdl.velocity.y) * TOP_DOWN_SCALE
			);
		}
		
		
		const math::Vec2 povForwardVec = math::angle_to_vec(pov.angle);
		math::Vec2 scaledPlayerDir = math::scale(povForwardVec, 100.f);
		SDL_SetRenderDrawColor(m_renderer, 255, 100, 0, 0xFF);
		// draw player forward vector
		SDL_RenderDrawLine(m_renderer, (int)pov.pos.x * TOP_DOWN_SCALE, (int)pov.pos.y * TOP_DOWN_SCALE, (int)(pov.pos.x + scaledPlayerDir.x) * TOP_DOWN_SCALE, (int)(pov.pos.y + scaledPlayerDir.y) * TOP_DOWN_SCALE);
		

		//m_impl->player.render(m_renderer);		
	}

	void RenderEngine::RenderSprite(const math::Transform & povTransform, const Sprite& sprite) const
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
			const int screenSpaceSpriteHeight = static_cast<int>(MAP_CELL_PX / distanceToSprite * DIST_PROJECTION_PLANE);

			// TODO: calculate this based on angle between player and sprite
			const int animID = 0;

			SDL_Rect dstRect;
			dstRect.h = screenSpaceSpriteHeight;
			dstRect.w = screenSpaceSpriteHeight;
			dstRect.x = (int)screenX - screenSpaceSpriteHeight / 2;
			dstRect.y = (int)screenYBottomOfSprite - screenSpaceSpriteHeight;
			
			RectContainer rectContainer = GetWallClippedSprite(crdVec, dstRect, distanceToSprite, 64 /* TODO: fix magic number */);
			
			// completely obstructed. Bail
			if (!rectContainer.display)
			{
				return;
			}

			dstRect = rectContainer.screenSpaceRect;
			SDL_Rect spriteSheetRect = rectContainer.spriteSheetRect;
			//printf("spritesheet x:%d, y:%d, w:%d, h:%d || screenspace x:%d, y:%d, w:%d, h:%d\n",
			//	spriteSheetRect.x,
			//	spriteSheetRect.y,
			//	spriteSheetRect.w,
			//	spriteSheetRect.h,
			//	dstRect.x,
			//	dstRect.y,
			//	dstRect.w,
			//	dstRect.h
			//);

			// find the forward vector for the enemy
			const math::Vec2 spriteForwardVec = math::angle_to_vec(sprite.m_transform.angle);

			// register vector between player and enemy
			RegisterTransitiveTopDownData(TopDownLine{ playerPos, playerToSprite, Color{255,255,255,255} });

			// register vector for enemy forward
			RegisterTransitiveTopDownData(TopDownLine{ sprite.m_transform.pos, math::angle_to_vec(sprite.m_transform.angle) * 100, Color{200,0,230,255} });

			// register vector for player forward

			// find the angle between player forward vec and enemy forward vec
			const float angleBetweenPlayerForwardAndEnemyForward = math::angle(playerToSprite, spriteForwardVec);

			printf("angle: %f\n", angleBetweenPlayerForwardAndEnemyForward);


			sprite.m_spritesheet->render(animID, &spriteSheetRect, &dstRect);
		}
	}
}

