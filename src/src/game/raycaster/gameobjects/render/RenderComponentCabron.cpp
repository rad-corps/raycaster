#include "RenderComponentCabron.h"
#include "RCGFMath.h"
#include <cassert>
#include "gameobjects/base/GameObject.h"
#include <iostream>

using math::PI;

namespace
{
	constexpr int SPRITESHEET_ROWS = 8;
	constexpr int SPRITESHEET_COLS = 8;
	constexpr double WALK_SPEED_MS = 350.0;
	constexpr double HIT_DURATION_MS = 100.0;
}

namespace game
{
	// column = rotation
	int RenderComponentCabron::CalculateSpriteAnimationIDColumn(float angle)
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

	// row = walk cycle animation
	int RenderComponentCabron::CalculateSpriteAnimationIDRow()
	{
		return 1;
	}

	void RenderComponentCabron::OnAnimationUpdate(AiAnimation aiAnimation)
	{
		// ignore animation updates while being hit. but add them to cache for when the hit is complete
		if (m_aiAnimation == AiAnimation::Hit)
		{
			m_aiAnimationCache = aiAnimation;
			return;
		}

		m_animationTimer = 0.0;
		m_aiAnimation = aiAnimation;
	}

	void RenderComponentCabron::Render(const RenderEngine& re, const math::Transform& pov, GameObject& gameObject, rcgf::SpriteSheet* spriteSheet, double deltatime)
	{
		m_animationTimer += deltatime;

		switch (m_aiAnimation)
		{
		case AiAnimation::Walking:
		{
			if (m_animationTimer > WALK_SPEED_MS)
			{
				m_animationTimer = 0.0;
				m_walkAnimationID = ++m_walkAnimationID > 4 ? 1 : m_walkAnimationID;
			}
			const math::Vec2 povToSprite = gameObject.m_transform.pos - pov.pos;
			const math::Vec2 spriteForwardVec = math::angle_to_vec(gameObject.m_transform.angle);
			const int animIDColumn = CalculateSpriteAnimationIDColumn(math::angle(povToSprite, spriteForwardVec));
			const int animID = m_walkAnimationID * SPRITESHEET_COLS + animIDColumn;

			re.RenderSprite(pov, gameObject.m_transform, spriteSheet, animID, 64);
			break;
		}
		case AiAnimation::GunDraw:
			re.RenderSprite(pov, gameObject.m_transform, spriteSheet, 48, 64);
			break;
		case AiAnimation::GunAim:
			re.RenderSprite(pov, gameObject.m_transform, spriteSheet, 49, 64);
			break;
		case AiAnimation::GunFire:
			re.RenderSprite(pov, gameObject.m_transform, spriteSheet, 50, 64);
			break;
		case AiAnimation::Hit:
		{
			if (m_animationTimer > HIT_DURATION_MS)
			{
				m_animationTimer = 0.0;
				m_aiAnimation = m_aiAnimationCache;
			}

			re.RenderSprite(pov, gameObject.m_transform, spriteSheet, 40, 64);
			break;
		}
		}
	}

	void RenderComponentCabron::OnEnemyDamage(const EnemyDamagePayload& payload)
	{
		// cache the previous animation if the new one is a hit
		if (m_aiAnimation != AiAnimation::Hit)
		{
			m_aiAnimationCache = m_aiAnimation;
		}
		m_aiAnimation = AiAnimation::Hit;
		m_animationTimer = 0.0;
	}

	void RenderComponentCabron::OnEnemyDeath(const EnemyDeathPayload& payload)
	{
	}
}