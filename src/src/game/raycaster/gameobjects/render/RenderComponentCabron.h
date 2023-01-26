#pragma once

#include "./gameobjects/base/RenderComponent.h"
#include <vector>
namespace game
{
	class RenderComponentCabron : public RenderComponent
	{
	public:
		void Render(const game::RenderEngine& re, const math::Transform& pov, GameObject& gameObject, rcgf::SpriteSheet* spriteSheet, double deltatime) override;
		virtual void OnAnimationUpdate(AiAnimation aiAnimation) override;
		void OnEnemyDamage(const EnemyDamagePayload& payload);
		void OnEnemyDeath();
	private:
		int CalculateSpriteAnimationIDColumn(float angle);
		int CalculateSpriteAnimationIDRow();

		double m_animationTimer;
		int m_walkAnimationID = 1;
		AiAnimation m_aiAnimation = AiAnimation::Walking;
		AiAnimation m_aiAnimationCache = AiAnimation::Walking;
	};
}