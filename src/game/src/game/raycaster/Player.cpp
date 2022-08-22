#pragma once

#include "Player.h"

namespace
{
	constexpr float X_START_POS = 20.f;
	constexpr float Y_START_POS = 20.f;
	constexpr float MOVEMENT_SPEED = 1.f;
	constexpr float ROTATION_SPEED = 0.03f;
}

namespace game
{
	Player::Player()
		: x{ X_START_POS }, y{ Y_START_POS }, angle{ 0.f }
	{}

	float Player::sumAngle(float add) const
	{
		add += angle;
		if (add < 0) add += PI * 2;
		else if (2 * PI < add) add -= PI * 2;
		return add;
	}

	void Player::render()
	{
		// draw the player
		int constexpr playerDiameter = 2;
		SDL_SetRenderDrawColor(global::instance.getRenderer(), 100, 200, 0, 0xFF);
		SDL_Rect r{ (int)x - playerDiameter,(int)y - playerDiameter,playerDiameter * 2,playerDiameter * 2 };
		SDL_RenderFillRect(global::instance.getRenderer(), &r);
	}

	void Player::rotate(RotateDirection dir)
	{
		if (dir == RotateDirection::Clockwise) angle += ROTATION_SPEED;
		else angle -= ROTATION_SPEED;
		if (angle < 0) angle += PI * 2;
		else if (2 * PI < angle) angle -= PI * 2;
	}

	void Player::move(bool forward)
	{
		float movementAngle = angle;
		if (!forward)
		{
			PI < movementAngle ? movementAngle -= PI : movementAngle += PI;
		}
		y += sin(movementAngle) * MOVEMENT_SPEED;
		x += cos(movementAngle) * MOVEMENT_SPEED;

		// TODO: collision checking
		// check column collision
		// checl row collision
	}
}