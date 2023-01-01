#pragma once

//#pragma warning( disable : 4189 ) // local variable is initialized but not referenced

#include "Globals.h"
#include "RCGFMath.h"
#include "Texture.h"
#include <iostream>

// #define RENDER_FLOORS

namespace game
{
	constexpr int MAP_CELL_PX = 8;
	constexpr int PLAYER_HEIGHT = MAP_CELL_PX / 2;
	constexpr int WALL_TEXTURE_SZ = 64;
	constexpr int WALL_TEXTURE_W = WALL_TEXTURE_SZ;
	constexpr int WALL_TEXTURE_H = WALL_TEXTURE_SZ;
	constexpr float FOV = math::PI / 3.f; // 60 degrees

	// 0.57735 is precomputed tan(FOV/2) : for screen width 320, result is 277
	constexpr int DIST_PROJECTION_PLANE = static_cast<int>((SCREEN_WIDTH / 2) / 0.57735); 
	constexpr int MAP_COLS = 32;
	constexpr int MAP_ROWS = 32;
	constexpr int MAX_X_BOUNDARY = MAP_CELL_PX * MAP_COLS;
	constexpr int MAX_Y_BOUNDARY = MAP_CELL_PX * MAP_ROWS;
	constexpr int MAP_SZ = MAP_COLS * MAP_ROWS;
	constexpr int X_PX_STEP = 1;
	constexpr int COLUMNS = SCREEN_WIDTH / X_PX_STEP;
	constexpr float START_ANGLE = 0.f;
	constexpr int TOP_DOWN_SCALE = 2;
	constexpr int GAME_OBJECT_ACTIVE_POOL_SZ = 1024; // arbitrary, lets tune this as we need to.

	constexpr int RANDOM_ENEMY_NUM = 50;

	constexpr float ACTOR_VELOCITY = 0.15f;

	struct ColumnRenderData
	{
		rcgf::Texture* columnTexture;
		SDL_Rect srcRect;
		SDL_Rect dstRect;
		float distance;

		// ray is currently only used for rendering the rays in the top down view. 
		// This could be removed entirely. Or set as a debug macro
		Line ray; 
	};

	enum Facing : unsigned char
	{
		UP = 0x0001,
		RIGHT = 0x0002,
		DOWN = 0x0004,
		LEFT = 0x0008,
	};

	struct RayWallCollision
	{
		float distance = 10000000.f;
		float positionAlongWall = 0.f; // between 0-1
		
		// wallFaceDir is currently unused, but may be needed 
		// if we change to rendering polys instead of columns
		Facing wallFaceDir = Facing::UP;
		
		// xHitPos and yHitPos technically not required, 
		// currently being used for displaying player rays in top down map
		float xHitPos = 0.f;
		float yHitPos = 0.f;
	};
}