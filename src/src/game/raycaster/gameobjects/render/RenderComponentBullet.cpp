#include "RenderComponentBullet.h"

namespace game
{
	void RenderComponentBullet::Render(const RenderEngine& re, const math::Transform& pov, const math::Transform& objTransform, rcgf::SpriteSheet* spriteSheet)
	{
		re.RenderSprite(pov, objTransform, spriteSheet, 0);
	}
}