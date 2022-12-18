#pragma once

#include "./gameobjects/base/RenderComponent.h"
namespace game
{
	class RenderComponentCabron : public RenderComponent
	{
	public:
		void Render(const game::RenderEngine& re, const math::Transform& pov, const math::Transform& objTransform, rcgf::SpriteSheet* spriteSheet) override;
	private:
		int CalculateSpriteAnimationID(float angle);
	};
}