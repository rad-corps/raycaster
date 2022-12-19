#include "RenderComponentCabron.h"
#include "RCGFMath.h"
#include <cassert>
#include "gameobjects/base/GameObject.h"

using math::PI;

namespace game
{
	int RenderComponentCabron::CalculateSpriteAnimationID(float angle)
	{
		// 0: front, 1: right, 2: back, 3: left

		if (angle >= 7 * PI / 4 || angle <= PI / 4)
		{
			return 2;
		}
		if (angle >= PI / 4 && angle <= 3 * PI / 4)
		{
			return 3;
		}
		if (angle >= 3 * PI / 4 && angle <= 5 * PI / 4)
		{
			return 0;
		}
		if (angle >= 5 * PI / 4 && angle <= 7 * PI / 4)
		{
			return 1;
		}

		assert(false);
		return -1;
	}

	void RenderComponentCabron::Render(const RenderEngine& re, const math::Transform& pov, GameObject& gameObject, rcgf::SpriteSheet* spriteSheet)
	{
		const math::Vec2 povToSprite = gameObject.m_transform.pos - pov.pos;
		const math::Vec2 spriteForwardVec = math::angle_to_vec(gameObject.m_transform.angle);
		const int animID = CalculateSpriteAnimationID(math::angle(povToSprite, spriteForwardVec));
		re.RenderSprite(pov, gameObject.m_transform, spriteSheet, animID, 64);
	}
}