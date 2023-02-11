#pragma once

#include "RaycasterConstants.h"
#include "RCGFMath.h"
#include <vector>
#include <cassert>
#include <array>
#include "Map.h"

namespace game::spatial
{
	bool line_of_sight(const math::Vec2& from, const math::Vec2& to);
	bool within_frustum(const math::Transform& pov, const math::Vec2& subject, float frustum = FOV);
	
	
	void init_path_finding(); // This must be called once before using do_pathfinding
	std::vector<math::Vec2> do_pathfinding(const math::Vec2& from, const math::Vec2& to);

	struct PathfindingGridCell
	{
		PathfindingGridCell(int mapIndex)
			: mapIndex{ mapIndex }
			, adjacentCells{ game::map::get_adjacent_map_indices(mapIndex) }
			, parent{ nullptr }
			, pos{ game::map::to_world_position(mapIndex) }
		{
			assert(mapIndex >= 0);

			// TODO: reintroduce assert
			//assert(mapIndex < game::MAP_SZ);
		}
		
		PathfindingGridCell()
			: mapIndex{ -1 }
			, adjacentCells{ -1, -1, -1, -1, -1, -1, -1, -1 }
			, pos{0.f,0.f}
		{}

		int Cost()
		{
			if (game::map::is_wall(mapIndex))
			{
				return 1000000;
			}
			else
			{
				return 1;
			}
		}

		bool operator==(const PathfindingGridCell& pc)
		{
			return mapIndex == pc.mapIndex;
		}

		PathfindingGridCell* parent;

		int mapIndex;
		float g = 0.f; // cummulative edge cost
		float f = 10000000.f; // heuristic (distance from current to goal)
		std::array<int, 8> adjacentCells;
		math::Vec2 pos;
	};

	PathfindingGridCell* get_pathfinding_grid_cell(int index);
	using PathfindingMap = std::vector<PathfindingGridCell>;
}