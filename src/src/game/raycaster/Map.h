#pragma once

#include <array>
#include "RaycasterConstants.h"

namespace game
{
	using GameMap = std::array<int, MAP_SZ>;

	bool facingDown(float angle_);
	bool facingUp(float angle_);
	bool facingRight(float angle_);
	bool facingLeft(float angle_);
	unsigned char getFacing(float angle_);

	int toMapIndex(float x, float y);
	int toMapIndex(int x, int y);
	bool isWall(float x, float y, const GameMap* map);
	bool isWall(int x, int y, const GameMap* map);
	bool isInWall(const SDL_Rect* playerVolume, const GameMap* map);
	const GameMap& getMap();

	RayWallCollision FindWallHitPos(const math::Vec2& pos, float rayAngle, const GameMap* map);
}