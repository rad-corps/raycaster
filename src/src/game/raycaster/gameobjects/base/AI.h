#pragma once

#include "RCGFMath.h"
#include <vector>
#include <memory>
#include "Map.h"

namespace game
{
	class GameObject;

	class AI
	{
	public:
		virtual ~AI() {}
		virtual std::unique_ptr<AI> Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap) = 0;
	};
}