#pragma once

#include <memory>
#include "Animation.h"
#include "AI.h"
#include "RCGFMath.h"

namespace game
{
	// create one of these per object in world
	struct Actor
	{
		Actor() = delete;
		Actor(rcgf::Animation* spritesheet, math::Transform transform, std::unique_ptr<AI> ai);
		void Update();

		math::Transform m_transform;
		rcgf::Animation* m_spritesheet;
		std::unique_ptr<AI> m_ai;
	};
} 