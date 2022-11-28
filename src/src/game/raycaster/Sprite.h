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
		Sprite(SDL_Renderer* renderer, rcgf::Animation* spritesheet, math::Transform transform) 
			: m_spritesheet{ spritesheet }
			, m_transform{ transform }
			, m_renderer{ renderer }
		{}

		math::Transform m_transform;
		rcgf::Animation* m_spritesheet;
		SDL_Renderer* m_renderer;

		void render(const math::Transform& povTransform) const;
	};
}