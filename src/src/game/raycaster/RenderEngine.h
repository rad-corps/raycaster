#pragma once

#include "Globals.h"
#include "RaycasterConstants.h"
#include <vector>
#include "Map.h"
#include "Sprite.h"

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
		void RenderTopDownMap(const game::GameMap& map, const math::Transform& pov, bool showRays);
		void RenderSprite(const math::Transform& povTransform, const Sprite& sprite) const;

	private:
		SDL_Renderer* m_renderer;
		const std::vector<ColumnRenderData>& crdVec;
		
		
	};
}

