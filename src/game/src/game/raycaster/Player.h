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
		void move(bool forward, GameMap* map);
		void strafe(bool right, GameMap* map);

		Transform transform;
	};
}