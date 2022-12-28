#pragma once

#include "RaycasterConstants.h"
#include <array>
#include <vector>
#include "Map.h"
#include "Texture.h"



namespace game
{
	bool facingDown(float angle_);
	bool facingUp(float angle_);
	bool facingRight(float angle_);
	bool facingLeft(float angle_);
	unsigned char getFacing(float angle_);

	class RaycastEngine
	{
	public:
		RaycastEngine() : wallData(SCREEN_WIDTH / X_PX_STEP) {}
		const std::vector<ColumnRenderData>& generateWallRenderData(math::Transform playerTransform, GameMap* map, rcgf::Texture* wallTexture);
		const std::vector<ColumnRenderData>& GetColumnRenderData()
		{
			return wallData;
		}
		RayWallCollision FindWallHitPos(const math::Transform& transform, float rayAngle, GameMap* map);

	private:	
		ColumnRenderData DoRayTest(const math::Transform& transform, float rayAngle, int pxCol, GameMap* map, rcgf::Texture* wallTexture);
		
		void DrawFloorColumn(SDL_Renderer* renderer, const math::Transform& playerTransform, const ColumnRenderData& crd, int screenColumnNumber, float rayAngle, rcgf::Texture* tx);
		
		std::vector<ColumnRenderData> wallData;
	};
}