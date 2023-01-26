#pragma once

#include "Globals.h"
#include <memory>
#include <vector>
#include "./gameobjects/base/GameObject.h"
#include "RCGFMath.h"

namespace game
{
	class GameObjectPool;
}

namespace game::factory
{
	void Init(SDL_Renderer* renderer, GameObjectPool* gameObjects);
	void CreateCabron(const math::Transform& transform, const std::vector<math::Vec2>& waypoints);
	void CreatePlayerBullet(const math::Transform& transform, GameObject* origin);
	void CreatePlayerBulletHitFX(const math::Transform& transform);
}
