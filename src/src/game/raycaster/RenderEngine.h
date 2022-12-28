#pragma once

#include "Globals.h"
#include "RaycasterConstants.h"
#include <vector>
#include "Map.h"
#include "SpriteSheet.h"
//#include "Actor.h"

namespace game
{
	class RenderEngine
	{
	public:
		// remove default/copy/move/assignment constructors
		RenderEngine() = delete;
		RenderEngine(const RenderEngine&) = delete;
		RenderEngine(RenderEngine&&) = delete;
		RenderEngine& operator=(const RenderEngine&) = delete;
		RenderEngine& operator=(RenderEngine&&) = delete;

		RenderEngine(SDL_Renderer* renderer, const std::vector<ColumnRenderData>& crdVec) 
			: m_renderer{ renderer }
			, crdVec{crdVec}
		{}

		void RenderWalls();

		void PushTopDownMapData(const TopDownLine& tdl) const 
		{
			if ( m_topDownMapActive ) topdownLineData.push_back(tdl);
		}

		void ClearTopDownMapData() const
		{
			if (m_topDownMapActive) topdownLineData.clear();
		}

		void SetTopDownMapActive(bool active)
		{
			m_topDownMapActive = active;
		}
		void RenderTopDownMap(const game::GameMap& map, const math::Transform& pov, bool showRays) const;
		void RenderSprite(const math::Transform& povTransform, const math::Transform& spriteTransform, rcgf::SpriteSheet* spriteSheet, int spriteSheetIdx, int spriteSz) const;
		SDL_Renderer* GetRenderer();

	private:
		mutable std::vector<TopDownLine> topdownLineData;
		SDL_Renderer* m_renderer;
		const std::vector<ColumnRenderData>& crdVec;
		bool m_topDownMapActive = false;
	};
}

