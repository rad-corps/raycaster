#pragma once

#include "./gameobjects/base/AI_Component.h"

namespace game
{
	class AI_ComponentBullet : public AI_Component
	{
	public:
		std::unique_ptr<AI_Component> Update(GameObject& subject, GameObjectPool& pool, const GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) override;
	};
}