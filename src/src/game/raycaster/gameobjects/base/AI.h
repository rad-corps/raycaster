#pragma once

#include "RCGFMath.h"
#include <vector>
#include <memory>
#include "Map.h"
#include "EventRecipient.h"

namespace game
{
	class GameObject;

	class AI : public EventRecipient
	{
	public:
		virtual ~AI() {}
		virtual std::unique_ptr<AI> Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap) = 0;
	};
}