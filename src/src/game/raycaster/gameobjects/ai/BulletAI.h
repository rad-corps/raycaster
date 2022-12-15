#pragma once

#include "gameobjects/base/AI.h"

namespace game
{
	class BulletBehavior : public AI
	{
	public:
		std::unique_ptr<AI> Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap) override;
	};
}