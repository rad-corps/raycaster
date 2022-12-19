#pragma once

#include "gameobjects/base/RenderComponent.h"
#include "gameobjects/base/GameObject.h"

namespace game
{
	class RenderComponentBullet : public RenderComponent
	{
	public:
		void Render(const game::RenderEngine& re, const math::Transform& pov, GameObject& objTransform, rcgf::SpriteSheet* spriteSheet) override;
	};
}