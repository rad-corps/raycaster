#pragma once

#include "gameobjects/base/RenderComponent.h"
#include "gameobjects/base/GameObject.h"

namespace game
{
	class RenderComponentBulletHitFX : public RenderComponent
	{
	public:
		void Render(const game::RenderEngine& re, const math::Transform& pov, GameObject& gameObject, rcgf::SpriteSheet* spriteSheet, double deltatime) override;
	private:
		double m_timer = 0.0;
		int animID = 1;
	};
}