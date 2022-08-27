#pragma once

#include <array>

namespace game
{
	constexpr int MAP_CELL_PX = 8;
	constexpr int MAP_COLS = 32;
	constexpr int MAP_ROWS = 32;
	constexpr int MAP_SZ = MAP_COLS * MAP_ROWS;

	using GameMap = std::array<int, MAP_SZ>;
}