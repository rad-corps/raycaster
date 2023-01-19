#pragma once

#include "EventTypes.h"
#include "RCGFMath.h"

namespace game
{
	class EventRecipient
	{
	public:
		virtual ~EventRecipient() = default;
		virtual void OnEnemyDamage(const EnemyDamagePayload&) {}
		virtual void OnEnemyDeath(const EnemyDeathPayload&) {}
		virtual void OnAlert(const math::Vec2& pos, const math::Vec2& alertPos) {}
		virtual void OnAnimationUpdate(AiAnimation aiAnimation) {}
	};
}
