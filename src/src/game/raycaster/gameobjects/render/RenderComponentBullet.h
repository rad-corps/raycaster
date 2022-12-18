#pragma once

#include "gameobjects/base/RenderComponent.h"

namespace game
{
	class RenderComponentBullet : public RenderComponent
	{
	public:
		void Render(const game::RenderEngine& re, const math::Transform& pov, const math::Transform& objTransform, rcgf::SpriteSheet* spriteSheet) override;
	};
}