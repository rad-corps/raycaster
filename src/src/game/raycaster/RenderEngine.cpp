#include "RenderEngine.h"
#include <cassert>
#include <iostream>
#include "EventSystem.h"
#include "../config/DesignerConstants.h"


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
	RenderEngine::RenderEngine(SDL_Renderer* renderer, const std::vector<ColumnRenderData>& crdVec)
		: m_renderer{ renderer }
		, crdVec{ crdVec }
	{
		events::subscribe(events::ColouredLineEvent::type, [this](const events::Event& event)
		{
			const events::ColouredLineEvent& derivedEvent = static_cast<const events::ColouredLineEvent&>(event);
			queuedColouredLines.push_back(derivedEvent.tdl);
		});

		events::subscribe(events::ColouredRectEvent::type, [this](const events::Event& event)
		{
			const events::ColouredRectEvent& derivedEvent = static_cast<const events::ColouredRectEvent&>(event);
			queuedColouredRects.push_back(derivedEvent.colouredRect);
		});
	}

	void RenderEngine::PushColouredLine(const ColouredLine& tdl)
	{
		if (m_topDownMapActive) queuedColouredLines.push_back(tdl);
	}

	void RenderEngine::PushColouredRect(const ColouredRect& cr)
	{
		if (m_topDownMapActive) queuedColouredRects.push_back(cr);
	}

	void RenderEngine::ClearTopDownMapData()
	{
		if (m_topDownMapActive)
		{
			queuedColouredLines.clear();
			queuedColouredRects.clear();
		}
	}

	void RenderEngine::SetTopDownMapActive(bool active)
	{
		m_topDownMapActive = active;
	}

	SDL_Renderer* RenderEngine::GetRenderer() const
	{
		return m_renderer;
	}

	void RenderEngine::RenderWalls() const
	{
		for (const ColumnRenderData& crd : crdVec)
		{
			// hack. calculate the srcRect elsewhere later
			// its currently 7 wide *groan*
			SDL_Rect tmpSrc;
			tmpSrc.x = (crd.wallSpritesheetIndex % design::WALL_SPRITESHEET_COLUMNS) * 64;
			tmpSrc.y = (crd.wallSpritesheetIndex / design::WALL_SPRITESHEET_COLUMNS) * 64;

			tmpSrc.x = tmpSrc.x + crd.srcRect.x;
			tmpSrc.y = tmpSrc.y + crd.srcRect.y;
			tmpSrc.w = crd.srcRect.w;
			tmpSrc.h = crd.srcRect.h;

			crd.columnTexture->render2(&tmpSrc, &crd.dstRect);
		}
	}

	void RenderEngine::RenderTopDownMap(const map::GameMap& map, const math::Transform& pov, bool showRays)
	{
		if (!m_topDownMapActive)
		{
			queuedColouredLines.clear();
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
				if (map[col + row * MAP_COLS] >= 0)
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
				SDL_RenderDrawLineF(m_renderer, crd.ray.start.x * TOP_DOWN_SCALE, crd.ray.start.y * TOP_DOWN_SCALE, crd.ray.end.x * TOP_DOWN_SCALE, crd.ray.end.y * TOP_DOWN_SCALE);
			}
		}

		// get player direction as a scaled vector
		const math::Vec2 povForwardVec = math::angle_to_vec(pov.angle);
		math::Vec2 scaledPlayerDir = math::scale(povForwardVec, 10.f);
		
		// draw player forward vector
		SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 0xFF);
		SDL_RenderDrawLineF(m_renderer, pov.pos.x * TOP_DOWN_SCALE, pov.pos.y * TOP_DOWN_SCALE, (pov.pos.x + scaledPlayerDir.x) * TOP_DOWN_SCALE, (pov.pos.y + scaledPlayerDir.y) * TOP_DOWN_SCALE);

		// draw a box around player
		{
			constexpr int sz = 6 * TOP_DOWN_SCALE;
			const SDL_FRect playerBox{
				pov.pos.x * TOP_DOWN_SCALE - sz / 2, // x
				pov.pos.y * TOP_DOWN_SCALE - sz / 2, // y
				sz,             // w
				sz              // h
			};
			SDL_RenderFillRectF(m_renderer, &playerBox);
		}

		// draw the queued data
		for (const auto& tdl : queuedColouredLines)
		{
			SDL_SetRenderDrawColor(m_renderer, tdl.color);
			SDL_RenderDrawLineF(
				m_renderer,
				tdl.line.start.x * TOP_DOWN_SCALE,
				tdl.line.start.y * TOP_DOWN_SCALE,
				tdl.line.end.x * TOP_DOWN_SCALE,
				tdl.line.end.y * TOP_DOWN_SCALE
			);
		}

		for (ColouredRect cr : queuedColouredRects)
		{
			cr.rect.x *= TOP_DOWN_SCALE;
			cr.rect.y *= TOP_DOWN_SCALE;
			cr.rect.w *= TOP_DOWN_SCALE;
			cr.rect.h *= TOP_DOWN_SCALE;
			SDL_SetRenderDrawColor(m_renderer, cr.color);
			SDL_RenderFillRectF(m_renderer, &cr.rect);
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

		// one current limitation is sprite width must equal height
		const float screenSpaceSpriteFullSz = MAP_CELL_PX / correctedDistanceToSprite * DIST_PROJECTION_PLANE;
		const float screenSpaceSpriteSz = MAP_CELL_PX / correctedDistanceToSprite * DIST_PROJECTION_PLANE * szMultiplier;

		// now we have an angle in radians from the player to the sprite that is 0 at far left of FOV, and FOV at far right.
		// convert angle to screen space
		const float screenXLeftOfSprite = ((leftFovToSpriteAngle / FOV) * SCREEN_WIDTH) - screenSpaceSpriteSz / 2;

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
		// we are currently assuming that we want the height centered. For example, bullets are 16px in size, and we want their height centered. 
		// This might need to be parameterised later.
		const float screenYTopOfSprite = (((DIST_PROJECTION_PLANE * PLAYER_HEIGHT) / correctedDistanceToSprite) + CENTER_Y) - (screenSpaceSpriteFullSz / 2.f + screenSpaceSpriteSz / 2.f);

		// TODO: this should be an SDL_FRect
		SDL_Rect dstRect{
			(int)screenXLeftOfSprite,
			(int)screenYTopOfSprite,
			(int)screenSpaceSpriteSz,
			(int)screenSpaceSpriteSz
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

