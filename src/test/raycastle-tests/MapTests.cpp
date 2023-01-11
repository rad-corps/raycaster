#include "pch.h"
#include "Map.h"

/**
*  [0][1][2]
*  [3]idx[4]
*  [5][6][7]
*/
TEST(TestMapIndices, TopLeftIndex) {
	auto ret = game::map::get_adjacent_map_indices(0);
	EXPECT_EQ(ret[0], -1);
	EXPECT_EQ(ret[1], -1);
	EXPECT_EQ(ret[2], -1);
	EXPECT_EQ(ret[3], -1);
	EXPECT_EQ(ret[4], 1);
	EXPECT_EQ(ret[5], -1);
	EXPECT_EQ(ret[6], 32);
	EXPECT_EQ(ret[7], 33);
}

TEST(TestMapIndices, RightTopIndex) {
	auto ret = game::map::get_adjacent_map_indices(31);
	EXPECT_EQ(ret[0], -1);
	EXPECT_EQ(ret[1], -1);
	EXPECT_EQ(ret[2], -1);
	EXPECT_EQ(ret[3], 30);
	EXPECT_EQ(ret[4], -1);
	EXPECT_EQ(ret[5], 31+32-1);
	EXPECT_EQ(ret[6], 31+32);
	EXPECT_EQ(ret[7], -1);
}

TEST(TestMapIndices, RightBottomIndex) {
	constexpr int idx = game::MAP_SZ - 1;
	auto ret = game::map::get_adjacent_map_indices(idx);
	EXPECT_EQ(ret[0], idx - 32 - 1);
	EXPECT_EQ(ret[1], idx - 32);
	EXPECT_EQ(ret[2], -1);
	EXPECT_EQ(ret[3], idx - 1);
	EXPECT_EQ(ret[4], -1);
	EXPECT_EQ(ret[5], -1);
	EXPECT_EQ(ret[6], -1);
	EXPECT_EQ(ret[7], -1);
}

TEST(TestMapIndices, LeftBottomIndex) {
	constexpr int idx = (game::MAP_ROWS - 1) * game::MAP_COLS;
	auto ret = game::map::get_adjacent_map_indices(idx);
	EXPECT_EQ(ret[0], -1);
	EXPECT_EQ(ret[1], idx - 32);
	EXPECT_EQ(ret[2], idx - 32 + 1);
	EXPECT_EQ(ret[3], -1);
	EXPECT_EQ(ret[4], idx + 1);
	EXPECT_EQ(ret[5], -1);
	EXPECT_EQ(ret[6], -1);
	EXPECT_EQ(ret[7], -1);
}

TEST(TestMapIndices, MiddleIndex) {
	constexpr int idx = game::MAP_SZ / 2 + game::MAP_ROWS / 2;
	auto ret = game::map::get_adjacent_map_indices(idx);
	EXPECT_EQ(ret[0], idx - 32 - 1);
	EXPECT_EQ(ret[1], idx - 32);
	EXPECT_EQ(ret[2], idx - 32 + 1);
	EXPECT_EQ(ret[3], idx - 1);
	EXPECT_EQ(ret[4], idx + 1);
	EXPECT_EQ(ret[5], idx + 32 - 1);
	EXPECT_EQ(ret[6], idx + 32);
	EXPECT_EQ(ret[7], idx + 32 + 1);
}

TEST(TestToMapIndex, Normal) {

}