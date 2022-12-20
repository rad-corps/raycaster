#include "RenderComponentBullet.h"

namespace game
{
	void RenderComponentBullet::Render(const RenderEngine& re, const math::Transform& pov, GameObject& gameObject, rcgf::SpriteSheet* spriteSheet, double deltatime)
	{
		re.RenderSprite(pov, gameObject.m_transform, spriteSheet, 0, 16);
	}
}