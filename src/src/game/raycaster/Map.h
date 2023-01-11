#pragma once

#include <array>
#include "RaycasterConstants.h"

// todo: namespace game::map
namespace game::map
{
	using GameMap = std::array<int, MAP_SZ>;

	bool facingDown(float angle_);
	bool facingUp(float angle_);
	bool facingRight(float angle_);
	bool facingLeft(float angle_);
	unsigned char getFacing(float angle_);

	/**
	*  [0][1][2]
	*  [3]idx[4]
	*  [5][6][7]
	*/
	// return -1 for unnavailable tiles
	std::array<int, 8> get_adjacent_map_indices(int idx);

	math::Vec2 to_world_position(int mapIndex);
	int to_map_index(const math::Vec2& pos);
	int to_map_index(int x, int y);
	bool is_wall(float x, float y);
	bool is_wall(int x, int y);
	bool is_wall(int index);
	bool is_in_wall(const SDL_Rect* playerVolume);
	const GameMap& get_map();
	RayWallCollision find_wall_hit_pos(const math::Vec2& pos, float rayAngle);
}