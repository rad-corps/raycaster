#include "Actor.h"
#include "AI.h"

namespace game
{
	Actor::Actor(rcgf::Animation* spritesheet, math::Transform transform, std::unique_ptr<AI> ai)
		: m_spritesheet{ spritesheet }
		, m_transform{ transform }
		, m_ai{ std::move(ai) }
	{}
}

