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

		int toMapIndex(float x, float y);
		const std::vector<ColumnRenderData>& generateWallRenderData(SDL_Renderer* renderer, math::Transform playerTransform, GameMap* map, bool showRays, rcgf::Texture* wallTexture);
		ColumnRenderData doRayTest(const math::Transform& transform, float rayAngle, int pxCol, GameMap* map, rcgf::Texture* wallTexture);
		void drawFloorColumn(SDL_Renderer* renderer, const math::Transform& playerTransform, const ColumnRenderData& crd, int screenColumnNumber, float rayAngle, rcgf::Texture* tx);
		SDL_Point worldSpaceToScreenSpace(const math::Transform& playerTransform, int x, int y);

	private:
		std::vector<ColumnRenderData> wallData;
	};
}