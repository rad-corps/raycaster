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
		RenderEngine(SDL_Renderer* renderer, const std::vector<ColumnRenderData>& crdVec);

		void RenderWalls() const;

		void PushColouredLine(const ColouredLine& tdl);
		void PushColouredRect(const ColouredRect& cr);
		void ClearTopDownMapData();
		void SetTopDownMapActive(bool active);

		// TODO: this should be const. currently non-const due to modifying the queued data to draw
		void RenderTopDownMap(const map::GameMap& map, const math::Transform& pov, bool showRays);

		void RenderSprite(const math::Transform& povTransform, const math::Transform& spriteTransform, rcgf::SpriteSheet* spriteSheet, int spriteSheetIdx, int spriteSz) const;
		SDL_Renderer* GetRenderer() const;

	private:
		std::vector<ColouredLine> queuedColouredLines;
		std::vector<ColouredRect> queuedColouredRects;
		SDL_Renderer* m_renderer;
		const std::vector<ColumnRenderData>& crdVec;
		bool m_topDownMapActive = false;
	};
}

