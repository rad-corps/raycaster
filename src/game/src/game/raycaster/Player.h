#pragma once

#include "Globals.h"

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
		void move(bool forward);

		float x;
		float y;
		float angle;
	};
}