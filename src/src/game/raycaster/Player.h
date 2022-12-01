#pragma once

#include "Globals.h"
#include "Map.h"
#include "RaycasterConstants.h"
#include "RCGFMath.h"

namespace game
{
	class Player
	{
	public:
		Player(const math::Transform& startingPos);
		Player() = delete;

		enum class RotateDirection
		{
			Clockwise,
			Anticlockwise
		};

		math::Vec2 getOffset(float angle, float distance) const;
		void rotate(RotateDirection dir);
		void move(float relativeAngle, const GameMap* map);

		math::Transform transform;
		SDL_Rect wallCollisionBox;
	};
}