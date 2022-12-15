#pragma once

#include "gameobjects/base/RenderingComponent.h"

namespace game
{
	class BulletRenderer : public RenderingComponent
	{
	public:
		void Render(const game::RenderEngine& re, const math::Transform& pov, const math::Transform& objTransform, rcgf::SpriteSheet* spriteSheet) override;
	};
}