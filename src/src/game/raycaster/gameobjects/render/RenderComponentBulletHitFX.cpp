#include "RenderComponentBulletHitFX.h"
#include "Globals.h"
#include <iostream>

namespace
{
	constexpr double FX_ANIM_SPEED_MS = 50.0;
}

namespace game
{
	void RenderComponentBulletHitFX::Render(const RenderEngine& re, const math::Transform& pov, GameObject& gameObject, rcgf::SpriteSheet* spriteSheet)
	{
		m_timer += global::Global::getDeltaTime();
		if (m_timer > FX_ANIM_SPEED_MS)
		{
			m_timer -= FX_ANIM_SPEED_MS;
			animID++;
			if (animID > 6)
			{
				gameObject.m_active = false;
				return;
			}
		}
		re.RenderSprite(pov, gameObject.m_transform, spriteSheet, animID, 16);
	}
}