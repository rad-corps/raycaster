#pragma once

#include "Globals.h"
#include "Map.h"
#include "RaycasterConstants.h"

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

		float sumAngle(float add) const;
		void render();

		void rotate(RotateDirection dir);
		void move(float relativeAngle, const GameMap* map);
		//void strafe(bool right, const GameMap* map);

		Transform transform;
		SDL_Rect wallCollisionBox;
	};
}