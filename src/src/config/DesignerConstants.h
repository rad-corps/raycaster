#pragma once

namespace design
{
	constexpr int WALL_SPRITESHEET_COLUMNS = 10;
	constexpr int WALL_SPRITESHEET_ROWS = 20;

	constexpr float PLAYER_MOVEMENT_SPEED = 1.2f;

	// changing PLAYER_ROTATION_SPEED will affect gamepad, mouse and keyboard rotation
	// best to leave this and change mouse sensitivity until I seperate it properly
	constexpr float PLAYER_ROTATION_SPEED = 0.04f;

	//thumbstick range is from -32768 and 32767
	constexpr int THUMBSTICK_DEADZONE = 8000;

	constexpr float MOUSE_SENSITIVITY = 0.05f;


	// lower number is higher fire rate.
	constexpr float UZI_FIRE_RATE = 100.0f;
	
	// this is the amount of time between pressing the button the first shot being fired
	// allows for raising gun animation
	constexpr float UZI_READY_SPEED = 25.0f;

	constexpr float UZI_BULLET_DAMAGE = 1.f;
	constexpr float ENEMY_HEALTH_CABRON = 4.9f;
}
