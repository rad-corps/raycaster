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
	constexpr int HALF_WALL_COLLISION_BOX_SZ = 3;
}

namespace game
{
	Player::Player()
		: transform{ X_START_POS, Y_START_POS, 0.f }
		, wallCollisionBox{ 
			static_cast<int>(X_START_POS) - HALF_WALL_COLLISION_BOX_SZ,
			static_cast<int>(Y_START_POS) - HALF_WALL_COLLISION_BOX_SZ,
			HALF_WALL_COLLISION_BOX_SZ * 2,
			HALF_WALL_COLLISION_BOX_SZ * 2
		}
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

		SDL_RenderDrawRect(global::instance.getRenderer(), &wallCollisionBox);
	}

	void Player::rotate(RotateDirection dir)
	{
		if (dir == RotateDirection::Clockwise) transform.angle += ROTATION_SPEED;
		else transform.angle -= ROTATION_SPEED;
		if (transform.angle < 0) transform.angle += PI * 2;
		else if (2 * PI < transform.angle) transform.angle -= PI * 2;
	}

	void Player::move(bool forward, const GameMap* map)
	{
		float movementAngle = transform.angle;
		if (!forward)
		{
			PI < movementAngle ? movementAngle -= PI : movementAngle += PI;
		}
		const float yDelta = sin(movementAngle) * MOVEMENT_SPEED;
		transform.y += yDelta;
		wallCollisionBox.y = static_cast<int>(transform.y) - HALF_WALL_COLLISION_BOX_SZ;
		if (isInWall(&wallCollisionBox, map))
		{
			transform.y -= yDelta;
			wallCollisionBox.y = static_cast<int>(transform.y) - HALF_WALL_COLLISION_BOX_SZ;
		}

		const float xDelta = cos(movementAngle) * MOVEMENT_SPEED;
		transform.x += xDelta;
		wallCollisionBox.x = static_cast<int>(transform.x) - HALF_WALL_COLLISION_BOX_SZ;
		if (isInWall(&wallCollisionBox, map))
		{
			transform.x -= xDelta;
			wallCollisionBox.x = static_cast<int>(transform.x) - HALF_WALL_COLLISION_BOX_SZ;
		}
	}

	void Player::strafe(bool right, const GameMap* map)
	{
		float movementAngle = transform.angle;
		if (right)
		{
			movementAngle += PI / 2;
			if (movementAngle > PI * 2) movementAngle -= PI * 2;
		}
		else
		{
			movementAngle -= PI / 2;
			if (movementAngle < 0) movementAngle += PI * 2;
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

		wallCollisionBox.x = static_cast<int>(transform.x) - HALF_WALL_COLLISION_BOX_SZ;
		wallCollisionBox.y = static_cast<int>(transform.y) - HALF_WALL_COLLISION_BOX_SZ;
	}
}