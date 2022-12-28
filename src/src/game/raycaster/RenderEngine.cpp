#include "RenderEngine.h"
#include <cassert>
#include <iostream>


namespace
{
	using math::PI;

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

	float calculate_sprite_angle_from_left_fov(float povAngle, const math::Vec2& povToSprite)
	{
		const math::Vec2 leftFovVec = math::angle_to_vec(math::sum_angle(povAngle, -game::FOV / 2));
		return math::angle(leftFovVec, povToSprite);
	}

float calculate_fisheye_corrected_distance(const math::Vec2& povToSprite, float povWorldRotation)
	{
		const float distanceToSprite = math::magnitude(povToSprite);
		const float angleDifference = math::vec_to_angle(povToSprite) - povWorldRotation;
		return distanceToSprite * cos(angleDifference);
	}
}

namespace game
{
	SDL_Renderer* RenderEngine::GetRenderer()
	{
		return m_renderer;
	}

	void RenderEngine::RenderWalls()
	{
		for (const ColumnRenderData& crd : crdVec)
		{
			crd.columnTexture->render2(&crd.srcRect, &crd.dstRect);
		}
	}

	void RenderEngine::RenderTopDownMap(const game::GameMap& map, const math::Transform& pov, bool showRays) const
	{
		if (!m_topDownMapActive)
		{
			topdownLineData.clear();
			return;
		}
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

		const math::Vec2 povForwardVec = math::angle_to_vec(pov.angle);
		math::Vec2 scaledPlayerDir = math::scale(povForwardVec, 100.f);
		
		// draw player forward vector
		SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 0xFF);
		SDL_RenderDrawLine(m_renderer, (int)pov.pos.x * TOP_DOWN_SCALE, (int)pov.pos.y * TOP_DOWN_SCALE, (int)(pov.pos.x + scaledPlayerDir.x) * TOP_DOWN_SCALE, (int)(pov.pos.y + scaledPlayerDir.y) * TOP_DOWN_SCALE);

		// draw the queued data
		for (const auto& tdl : topdownLineData)
		{
			SDL_SetRenderDrawColor(m_renderer, tdl.color);
			SDL_RenderDrawLine(
				m_renderer,
				tdl.line.start.x * TOP_DOWN_SCALE,
				tdl.line.start.y * TOP_DOWN_SCALE,
				tdl.line.end.x * TOP_DOWN_SCALE,
				tdl.line.end.y * TOP_DOWN_SCALE
			);

		}
	}

	void RenderEngine::RenderSprite(const math::Transform& povTransform, const math::Transform& spriteTransform, rcgf::SpriteSheet* spriteSheet, int spriteSheetIdx, int spriteSz) const
	{
		const math::Vec2 povToSprite = spriteTransform.pos - povTransform.pos;
		const float leftFovToSpriteAngle = calculate_sprite_angle_from_left_fov(povTransform.angle, povToSprite);

		// cull actors that are off screen
		if (leftFovToSpriteAngle < 0 || leftFovToSpriteAngle > FOV)
		{
			// TODO: we need to be a bit smarter about < 0 off the left of screen.
			return;
		}
		
		const float correctedDistanceToSprite = calculate_fisheye_corrected_distance(povToSprite, povTransform.angle);
		if (correctedDistanceToSprite < 0.01f)
		{
			return; // avoid divide by 0
		}

		const float szMultiplier = spriteSz / 64.f;

		const int screenSpaceSpriteHeight = static_cast<int>(MAP_CELL_PX / correctedDistanceToSprite * DIST_PROJECTION_PLANE * szMultiplier);

		// now we have an angle in radians from the player to the sprite that is 0 at far left of FOV, and FOV at far right.
		// convert angle to screen space
		const float screenXLeftOfSprite = ((leftFovToSpriteAngle / FOV) * SCREEN_WIDTH) - screenSpaceSpriteHeight / 2;

		// ==========================================================================================================
		//                         To calculate the screen Y pixel position of a sprite
		// ==========================================================================================================
		// 
		//     distance from player to enemy                                     player height                       
		// ----------------------------------------  =   ------------------------------------------------------------
		// distance from player to projection plane         screen y pixel position - center y screen pixel position
		//
		// 
		// solving for screen y pixel position below
		const float screenYTopOfSprite = (((DIST_PROJECTION_PLANE * PLAYER_HEIGHT) / correctedDistanceToSprite) + CENTER_Y) - screenSpaceSpriteHeight;

		SDL_Rect dstRect{
			(int)screenXLeftOfSprite,
			(int)screenYTopOfSprite,
			screenSpaceSpriteHeight,
			screenSpaceSpriteHeight
		};

		RectContainer rectContainer = GetWallClippedSprite(crdVec, dstRect, correctedDistanceToSprite, spriteSz);

		// completely obstructed. Bail
		if (!rectContainer.display)
		{
			return;
		}

		dstRect = rectContainer.screenSpaceRect;
		SDL_Rect spriteSheetRect = rectContainer.spriteSheetRect;
		spriteSheet->render(spriteSheetIdx, &spriteSheetRect, &dstRect);
	}
}

