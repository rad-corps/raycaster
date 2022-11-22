#pragma once

#include "Player.h"
#include "Map.h"
#include <iostream>

namespace
{
	constexpr float X_START_POS = 20.f;
	constexpr float Y_START_POS = 64.f;
	constexpr float MOVEMENT_SPEED = 0.25f;
	constexpr float ROTATION_SPEED = 0.03f;
	//constexpr float ROTATION_SPEED = 0.001f;
	constexpr int HALF_WALL_COLLISION_BOX_SZ = 3;
}

namespace game
{
	Player::Player()
		: transform{ math::Vec2{ X_START_POS, Y_START_POS}, 0.f }
		, wallCollisionBox{ 
			static_cast<int>(X_START_POS) - HALF_WALL_COLLISION_BOX_SZ,
			static_cast<int>(Y_START_POS) - HALF_WALL_COLLISION_BOX_SZ,
			HALF_WALL_COLLISION_BOX_SZ * 2,
			HALF_WALL_COLLISION_BOX_SZ * 2
		}
	{
	}

	math::Vec2 Player::getOffset(float angle, float distance) const
	{
		return math::Vec2{ cos(angle) * distance, sin(angle) * distance };
	}

	float Player::sumAngle(float add) const
	{
		add += transform.angle;
		if (add < 0) add += PI * 2;
		else if (2 * PI < add) add -= PI * 2;
		return add;
	}

	void Player::render(SDL_Renderer* renderer)
	{
		// draw the player (for the top down map)
		constexpr int playerDiameter = 2;
		const SDL_Rect r{ (int)transform.pos.x * 2 - playerDiameter,(int)transform.pos.y * 2 - playerDiameter,playerDiameter * 2,playerDiameter * 2 };
		SDL_SetRenderDrawColor(renderer, 100, 200, 0, 0xFF);
		SDL_RenderFillRect(renderer, &r);

		//SDL_RenderDrawRect(renderer, &wallCollisionBox);
	}

	void Player::rotate(RotateDirection dir)
	{
		if (dir == RotateDirection::Clockwise)
		{
			transform.angle += ROTATION_SPEED;
		}
		else
		{
			transform.angle -= ROTATION_SPEED;
		}
		if (transform.angle < 0)
		{
			transform.angle += PI * 2;
		}
		else if (2 * PI < transform.angle)
		{
			transform.angle -= PI * 2;
		}
	}
	// relativeAngle = 0             : move forward
	// relativeAngle = PI            : move backward
	// relativeAngle = PI / 2.f      : strafe right
	// relativeAngle = PI + PI / 2.f : strafe left
	void Player::move(float relativeAngle, const GameMap* map)
	{
		float movementAngle = transform.angle + relativeAngle;
		if (movementAngle > PI * 2) movementAngle -= PI * 2;
		if (movementAngle < 0.f) movementAngle += PI * 2;

		const float yDelta = sin(movementAngle) * MOVEMENT_SPEED;
		transform.pos.y += yDelta;
		wallCollisionBox.y = static_cast<int>(transform.pos.y) - HALF_WALL_COLLISION_BOX_SZ;
		if (isInWall(&wallCollisionBox, map))
		{
			transform.pos.y -= yDelta;
			wallCollisionBox.y = static_cast<int>(transform.pos.y) - HALF_WALL_COLLISION_BOX_SZ;
		}

		const float xDelta = cos(movementAngle) * MOVEMENT_SPEED;
		transform.pos.x += xDelta;
		wallCollisionBox.x = static_cast<int>(transform.pos.x) - HALF_WALL_COLLISION_BOX_SZ;
		if (isInWall(&wallCollisionBox, map))
		{
			transform.pos.x -= xDelta;
			wallCollisionBox.x = static_cast<int>(transform.pos.x) - HALF_WALL_COLLISION_BOX_SZ;
		}
	}
}