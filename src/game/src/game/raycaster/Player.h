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
		Player();

		enum class RotateDirection
		{
			Clockwise,
			Anticlockwise
		};

		math::Vec2 getOffset(float angle, float distance) const;
		float sumAngle(float add) const;
		void render(SDL_Renderer* renderer);

		void rotate(RotateDirection dir);
		void move(float relativeAngle, const GameMap* map);

		Transform transform;
		SDL_Rect wallCollisionBox;
	};
}