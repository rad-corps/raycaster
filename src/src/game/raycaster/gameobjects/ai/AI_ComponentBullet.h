#pragma once

#include "./gameobjects/base/AI_Component.h"

namespace game
{
	class AI_ComponentBullet : public AI_Component
	{
	public:
		std::unique_ptr<AI_Component> Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap) override;
	};
}