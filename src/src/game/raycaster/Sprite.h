#pragma once

#include <memory>
#include "Animation.h"
#include "RCGFMath.h"

namespace game
{
	struct Sprite
	{
		math::Transform transform;
		rcgf::Animation* spritesheet;
	};

	void render_sprite(Sprite const* sprite, math::Transform povTransform);
}