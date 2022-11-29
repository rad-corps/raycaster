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

		RenderEngine(SDL_Renderer* renderer) : m_renderer{ renderer } {}

		void RenderWalls(const std::vector<ColumnRenderData>& crd);
		void RenderTopDownMap(const game::GameMap& map, const std::vector<ColumnRenderData>& crd, const math::Transform& pov, const math::Transform& refTransform, bool showRays);
		void RenderSprite(const math::Transform& povTransform, const Sprite& sprite) const;

	private:
		SDL_Renderer* m_renderer;

	};
}

