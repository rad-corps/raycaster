#pragma once

#include <memory>
#include "SpriteSheet.h"
#include "AI.h"
#include "RCGFMath.h"
#include "RenderingComponent.h"
#include "RenderEngine.h"

namespace game
{
	// create one of these per object in world
	struct Actor
	{
		Actor() = delete;
		Actor(rcgf::SpriteSheet* spritesheet, math::Transform transform, std::unique_ptr<AI> ai, std::unique_ptr<RenderingComponent> rc);
		void Update();
		void Render(const game::RenderEngine& re, const math::Transform& pov);

		math::Transform m_transform;
		rcgf::SpriteSheet* m_spritesheet;
		std::unique_ptr<AI> m_ai;
		std::unique_ptr<RenderingComponent> m_rc;
	};
} 