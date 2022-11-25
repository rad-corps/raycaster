#pragma once

#include "RaycasterConstants.h"
#include <array>
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

	//struct WallMapFace
	//{
	//	WallMapFace()
	//		: mapIndex(INT_MAX)
	//		, facing(CHAR_MAX)
	//	{}
	//	friend bool operator==(const WallMapFace& lhs, const WallMapFace& rhs);
	//	friend bool operator!=(const WallMapFace& lhs, const WallMapFace& rhs);
	//	int mapIndex;
	//	char facing;
	//};

	struct ColumnRenderData
	{
		int textureXPos;
		SDL_Rect rect;
		//Color color;
		Line ray;
	};

	namespace raycast_engine
	{
		int toMapIndex(float x, float y);
		ColumnRenderData doRayTest(const math::Transform& transform, float rayAngle, int pxCol, GameMap* map);
		void drawFloorColumn(SDL_Renderer* renderer, const math::Transform& playerTransform, const ColumnRenderData& crd, int screenColumnNumber, float rayAngle, rcgf::Texture* tx);
		SDL_Point worldSpaceToScreenSpace(const math::Transform& playerTransform, int x, int y);
	};
}