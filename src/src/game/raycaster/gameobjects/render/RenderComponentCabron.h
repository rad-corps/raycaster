#pragma once

#include "./gameobjects/base/RenderComponent.h"
#include <vector>
namespace game
{
	class RenderComponentCabron : public RenderComponent
	{
	public:
		void Render(const game::RenderEngine& re, const math::Transform& pov, GameObject& gameObject, rcgf::SpriteSheet* spriteSheet, double deltatime) override;
		void OnLineDraw(const TopDownLine& tdl) override;
	private:
		int CalculateSpriteAnimationIDColumn(float angle);
		int CalculateSpriteAnimationIDRow();

		double animationTimer;
		int walkAnimationID = 1;

		std::vector<TopDownLine> m_queue;
	};
}