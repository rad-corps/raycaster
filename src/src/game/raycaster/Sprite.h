#pragma once

#include <memory>
#include "Animation.h"
#include "RCGFMath.h"

namespace game
{
	// create one of these per object in world
	struct Sprite
	{
		Sprite() = delete;
		Sprite(rcgf::Animation* spritesheet, math::Transform transform) 
			: m_spritesheet{ spritesheet }
			, m_transform{ transform }
		{}

		math::Transform m_transform;
		rcgf::Animation* m_spritesheet;
	};
}