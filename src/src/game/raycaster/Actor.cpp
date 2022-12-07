#include "Actor.h"
#include "AI.h"

namespace game
{
	Actor::Actor(rcgf::Animation* spritesheet, math::Transform transform, std::unique_ptr<AI> ai)
		: m_spritesheet{ spritesheet }
		, m_transform{ transform }
		, m_ai{ std::move(ai) }
	{}

	void Actor::Update()
	{
		std::unique_ptr<AI> newAI = m_ai->Update(*this);
		if (newAI)
		{
			m_ai = std::move(newAI);
		}
	}
}

