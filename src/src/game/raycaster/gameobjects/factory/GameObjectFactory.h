#pragma once

#include "Globals.h"
#include <memory>
#include <vector>
#include "./gameobjects/base/GameObject.h"
#include "RCGFMath.h"

namespace game::factory
{
	void Init(SDL_Renderer* renderer);
	GameObject CreateCabron(const math::Transform& transform);
	GameObject CreateCabron(const math::Transform& transform, const std::vector<math::Vec2>& waypoints);
	GameObject CreatePlayerBullet(const math::Transform& transform, GameObject* origin);
	GameObject CreatePlayerBulletHitFX(const math::Transform& transform);
}
