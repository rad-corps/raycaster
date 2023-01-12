#include "pch.h"
#include "Spatial.h"
#include "Map.h"

#include <windows.h> // OutputDebugString

/**
*  [0][1][2]
*  [3]idx[4]
*  [5][6][7]
*/
TEST(TestToMapIndex, Normal) {

	
	OutputDebugStringA("TestToMapIndex output");

	game::spatial::init_path_finding();
	math::Vec2 startPos{ 8.f, 8.f };
	int idx = game::map::to_map_index(startPos);
	std::array<int, 8> adjacentMapIndices = game::map::get_adjacent_map_indices(idx);

	for (int mapIndex : adjacentMapIndices)
	{
		if (mapIndex != -1)
		{
			game::spatial::PathfindingGridCell* cell = game::spatial::get_pathfinding_grid_cell(mapIndex);
			EXPECT_TRUE(cell != nullptr);
		}
	}
}

TEST(ToWorldPositionTests, TopLeftPosition) {
	math::Vec2 worldPos = game::map::to_world_position(0);
	EXPECT_TRUE(worldPos == math::Vec2(0.f,0.f));
}

TEST(ToWorldPositionTests, TopRightPosition) {
	math::Vec2 worldPos = game::map::to_world_position(31);
	EXPECT_TRUE(worldPos == math::Vec2(248.f, 0.f));
}

TEST(ToWorldPositionTests, SecondRowFarLeft) {
	math::Vec2 worldPos = game::map::to_world_position(32);
	EXPECT_TRUE(worldPos == math::Vec2(0.f, 8.f));
}

TEST(ToWorldPositionTests, SecondRowFarRight) {
	math::Vec2 worldPos = game::map::to_world_position(63);
	EXPECT_TRUE(worldPos == math::Vec2(248.f, 8.f));
}