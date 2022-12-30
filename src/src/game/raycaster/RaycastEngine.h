#pragma once

#include "RaycasterConstants.h"
#include <array>
#include <vector>
#include "Map.h"
#include "Texture.h"



namespace game
{
	class RaycastEngine
	{
	public:
		RaycastEngine() : wallData(SCREEN_WIDTH / X_PX_STEP) {}
		const std::vector<ColumnRenderData>& generateWallRenderData(const math::Transform& playerTransform, const GameMap* map, rcgf::Texture* wallTexture);
		const std::vector<ColumnRenderData>& GetColumnRenderData()
		{
			return wallData;
		}
		
	private:	
		ColumnRenderData DoRayTest(const math::Transform& transform, float rayAngle, int pxCol, const GameMap* map, rcgf::Texture* wallTexture);
		
		void DrawFloorColumn(SDL_Renderer* renderer, const math::Transform& playerTransform, const ColumnRenderData& crd, int screenColumnNumber, float rayAngle, rcgf::Texture* tx);
		
		std::vector<ColumnRenderData> wallData;
	};
}