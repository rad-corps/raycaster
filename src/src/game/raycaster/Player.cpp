#pragma once

#include "Player.h"
#include "Map.h"
#include <iostream>
#include "../config/DesignerConstants.h"

using math::PI;

namespace
{
	// constexpr float MOVEMENT_SPEED = 0.6f;
	// constexpr float ROTATION_SPEED = 0.02f;
	constexpr int HALF_WALL_COLLISION_BOX_SZ = 3;
}

namespace game
{
	Player::Player(const math::Transform& startingPos)
		: transform{ startingPos }
		, wallCollisionBox{ 
			static_cast<int>(transform.pos.x) - HALF_WALL_COLLISION_BOX_SZ,
			static_cast<int>(transform.pos.y) - HALF_WALL_COLLISION_BOX_SZ,
			HALF_WALL_COLLISION_BOX_SZ * 2,
			HALF_WALL_COLLISION_BOX_SZ * 2
		}
	{
	}

	void Player::setTransform(const math::Transform& transform_)
	{
		transform = transform_;
	}

	math::Vec2 Player::getOffset(float angle, float distance) const
	{
		return math::Vec2{ cos(angle) * distance, sin(angle) * distance };
	}

	void Player::rotate(RotateDirection dir, float speed)
	{
		if (dir == RotateDirection::Clockwise)
		{
			transform.angle += design::PLAYER_ROTATION_SPEED * speed;
		}
		else
		{
			transform.angle -= design::PLAYER_ROTATION_SPEED * speed;
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
	void Player::move(float relativeAngle, const map::GameMap* gameMap, float speed)
	{
		float movementAngle = transform.angle + relativeAngle;
		if (movementAngle > PI * 2) movementAngle -= PI * 2;
		if (movementAngle < 0.f) movementAngle += PI * 2;

		const float yDelta = sin(movementAngle) * design::PLAYER_MOVEMENT_SPEED * speed;
		transform.pos.y += yDelta;
		wallCollisionBox.y = static_cast<int>(transform.pos.y) - HALF_WALL_COLLISION_BOX_SZ;
		if (map::is_in_wall(&wallCollisionBox))
		{
			transform.pos.y -= yDelta;
			wallCollisionBox.y = static_cast<int>(transform.pos.y) - HALF_WALL_COLLISION_BOX_SZ;
		}

		const float xDelta = cos(movementAngle) * design::PLAYER_MOVEMENT_SPEED * speed;
		transform.pos.x += xDelta;
		wallCollisionBox.x = static_cast<int>(transform.pos.x) - HALF_WALL_COLLISION_BOX_SZ;
		if (map::is_in_wall(&wallCollisionBox))
		{
			transform.pos.x -= xDelta;
			wallCollisionBox.x = static_cast<int>(transform.pos.x) - HALF_WALL_COLLISION_BOX_SZ;
		}
	}
}