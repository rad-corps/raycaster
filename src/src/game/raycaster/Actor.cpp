#include "Actor.h"
#include "AI.h"

namespace game
{
	Actor::Actor(rcgf::SpriteSheet* spritesheet, math::Transform transform, std::unique_ptr<AI> ai, std::unique_ptr<RenderingComponent> rc)
		: m_spritesheet{ spritesheet }
		, m_transform{ transform }
		, m_ai{ std::move(ai) }
		, m_rc{ std::move(rc) }
	{}

	void Actor::Update()
	{
		std::unique_ptr<AI> newAI = m_ai->Update(*this);
		if (newAI)
		{
			m_ai = std::move(newAI);
		}
	}

	void Actor::Render(const game::RenderEngine& re, const math::Transform& pov)
	{
		m_rc->Render(re, pov, m_transform, m_spritesheet);
	}
}

