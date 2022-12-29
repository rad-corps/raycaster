#pragma once

#include "EventTypes.h"

namespace game
{
	class EventRecipient
	{
	public:
		virtual ~EventRecipient() = default;
		virtual void OnEnemyDamage(const EnemyDamagePayload&) {}
		virtual void OnEnemyDeath(const EnemyDeathPayload&) {}
	};
}
