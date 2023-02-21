#pragma once

namespace design
{
	constexpr int WALL_SPRITESHEET_COLUMNS = 10;
	constexpr int WALL_SPRITESHEET_ROWS = 20;

	constexpr float PLAYER_MOVEMENT_SPEED = 1.2f;
	constexpr float PLAYER_ROTATION_SPEED = 0.04f;

	//thumbstick range is from -32768 and 32767
	constexpr int THUMBSTICK_DEADZONE = 8000;
}