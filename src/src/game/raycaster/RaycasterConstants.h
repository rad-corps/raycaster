#pragma once

//#pragma warning( disable : 4189 ) // local variable is initialized but not referenced

// unfortunately this gets in the way when throwing away params for some derived/overridden functions. 
#pragma warning( disable : 4100 ) // unreferenced formal parameter

#include "Globals.h"
#include "RCGFMath.h"
#include "Texture.h"

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

	constexpr int RANDOM_ENEMY_NUM = 50;

	constexpr float ACTOR_VELOCITY = 0.15f;

	struct ColumnRenderData
	{
		rcgf::Texture* columnTexture;
		SDL_Rect srcRect;
		SDL_Rect dstRect;
		Line ray;
		float distance;
	};
}