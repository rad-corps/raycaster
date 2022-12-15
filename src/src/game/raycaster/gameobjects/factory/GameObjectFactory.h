#pragma once

#include "Globals.h"
#include <memory>
#include "./gameobjects/base/GameObject.h"
#include "RCGFMath.h"

namespace game
{
	class GameObjectFactory
	{
	public:
		GameObjectFactory(SDL_Renderer* renderer);
		GameObject CreateCabron(const math::Transform& transform);
		GameObject CreatePlayerBullet(const math::Transform& transform);
	};
	
}
