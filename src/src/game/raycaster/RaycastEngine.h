#pragma once

#include "RaycasterConstants.h"
#include <array>
#include <vector>
#include "Map.h"
#include "Texture.h"



namespace game
{
	enum Facing : unsigned char
	{
		UP = 0x0001,
		RIGHT = 0x0002,
		DOWN = 0x0004,
		LEFT = 0x0008,
	};

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

	private:	
		ColumnRenderData doRayTest(const math::Transform& transform, float rayAngle, int pxCol, GameMap* map, rcgf::Texture* wallTexture);
		void drawFloorColumn(SDL_Renderer* renderer, const math::Transform& playerTransform, const ColumnRenderData& crd, int screenColumnNumber, float rayAngle, rcgf::Texture* tx);
		
		std::vector<ColumnRenderData> wallData;
	};
}