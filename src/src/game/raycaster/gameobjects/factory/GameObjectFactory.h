#pragma once

#include "Globals.h"
#include <memory>
#include "./gameobjects/base/GameObject.h"
#include "RCGFMath.h"

namespace game::factory
{
	void Init(SDL_Renderer* renderer);
	GameObject CreateCabron(const math::Transform& transform);
	GameObject CreatePlayerBullet(const math::Transform& transform);
	GameObject CreatePlayerBulletHitFX(const math::Transform& transform);
}
