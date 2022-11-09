#include "RaycasterConstants.h"

namespace game
{
	game::Vec2 angleToVec(float angle)
	{
		return game::Vec2{ cos(angle), sin(angle) };
	}
}