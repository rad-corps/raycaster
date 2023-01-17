#pragma once

#include "./gameobjects/base/AI_Component.h"

namespace game
{
	class GameObject;

	class AI_ComponentBullet : public AI_Component
	{
	public:
		AI_ComponentBullet(GameObject* origin)
			: origin{origin}
		{

		}
		std::unique_ptr<AI_Component> Update(GameObject& subject, GameObjectPool& pool, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) override;
	private:
		GameObject* origin;
	};
}