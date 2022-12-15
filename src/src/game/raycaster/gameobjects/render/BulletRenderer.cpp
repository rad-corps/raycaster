#include "BulletRenderer.h"
#include <cassert>

namespace game
{
	void BulletRenderer::Render(const RenderEngine& re, const math::Transform& pov, const math::Transform& objTransform, rcgf::SpriteSheet* spriteSheet)
	{
		re.RenderSprite(pov, objTransform, spriteSheet, 0);
	}
}