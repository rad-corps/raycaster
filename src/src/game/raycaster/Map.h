#pragma once

#include <array>
#include <vector>
#include "RaycasterConstants.h"

namespace game
{
	class Player;
}

// todo: namespace game::map
namespace game::map
{
	// using GameMap = std::array<int, MAP_SZ>;

	struct GameMap
	{
		GameMap() 
			: m_cols{ 0 }
			, m_rows{ 0 }
		{}

		GameMap(int cols, int rows)
			: m_cols{cols}
			, m_rows{rows}
		{
			m_mapData.resize(m_cols * m_rows);
		}

		int& operator[](int index)
		{
			return m_mapData[index];
		}

		const int& operator[](int index) const
		{
			return m_mapData[index];
		}

		int MAP_COLS() const { return m_cols;  }
		int MAP_ROWS() const { return m_rows; }
		int MAP_SZ() const { return m_rows * m_cols; }
		
	private:
		std::vector<int> m_mapData;
		int m_cols;
		int m_rows;
	};

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
	int get_wall_spritesheet_index(int x, int y);
	bool is_wall(float x, float y);
	bool is_wall(int x, int y);
	bool is_wall(int index);
	bool is_in_wall(const SDL_Rect* playerVolume);
	const GameMap& get_map();
	RayWallCollision find_wall_hit_pos(const math::Vec2& pos, float rayAngle);
	void set_map(const std::string& fileName, Player& player);
}