#pragma once

#include "RCGFMath.h"
#include <vector>
#include <memory>
#include "Map.h"
#include "EventRecipient.h"

namespace game
{
	class GameObject;
	class GameObjectPool;

	class AI_Component : public EventRecipient
	{
	public:
		virtual ~AI_Component() {}
		virtual std::unique_ptr<AI_Component> Update(GameObject& subject, GameObjectPool& pool, const GameMap& gameMap) = 0;
	};
}