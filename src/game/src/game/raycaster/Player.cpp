#pragma once

#include "Player.h"
#include "Map.h"
#include <iostream>

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
		: transform{ X_START_POS, Y_START_POS, 0.f }
	{}

	float Player::sumAngle(float add) const
	{
		add += transform.angle;
		if (add < 0) add += PI * 2;
		else if (2 * PI < add) add -= PI * 2;
		return add;
	}

	void Player::render()
	{
		// draw the player
		constexpr int playerDiameter = 2;
		const SDL_Rect r{ (int)transform.x - playerDiameter,(int)transform.y - playerDiameter,playerDiameter * 2,playerDiameter * 2 };
		SDL_SetRenderDrawColor(global::instance.getRenderer(), 100, 200, 0, 0xFF);
		SDL_RenderFillRect(global::instance.getRenderer(), &r);
	}

	void Player::rotate(RotateDirection dir)
	{
		if (dir == RotateDirection::Clockwise) transform.angle += ROTATION_SPEED;
		else transform.angle -= ROTATION_SPEED;
		if (transform.angle < 0) transform.angle += PI * 2;
		else if (2 * PI < transform.angle) transform.angle -= PI * 2;
	}

	void Player::move(bool forward, GameMap* map)
	{
		float movementAngle = transform.angle;
		if (!forward)
		{
			PI < movementAngle ? movementAngle -= PI : movementAngle += PI;
		}
		const float yDelta = sin(movementAngle) * MOVEMENT_SPEED;
		transform.y += yDelta;
		if (isWall(transform.x, transform.y, map))
		{
			transform.y -= yDelta;
		}

		const float xDelta = cos(movementAngle) * MOVEMENT_SPEED;
		transform.x += xDelta;
		if (isWall(transform.x, transform.y, map))
		{
			transform.x -= xDelta;
		}
	}
}