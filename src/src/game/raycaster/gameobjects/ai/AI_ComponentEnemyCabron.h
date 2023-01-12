#pragma once

#include "gameobjects/base/AI_Component.h"

namespace game
{
	class AI_WaypointFollow : public AI_Component
	{
	public:
		AI_WaypointFollow() = delete;
		AI_WaypointFollow(std::vector<math::Vec2> waypointPositions)
			: m_waypointPositions{ waypointPositions }
		{}

		std::unique_ptr<AI_Component> Update(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) override;
		void OnAlert(const math::Vec2& pos, const math::Vec2& alertPos) override;

		float m_timer = 10.f;

	private:
		void SetWaypointPositions(std::vector<math::Vec2>&& waypointPositions);
		std::vector<math::Vec2> m_waypointPositions;
		int m_waypointIndex = 0;
	};

	class AI_Empty : public AI_Component
	{
	public:
		std::unique_ptr<AI_Component> Update(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) override;
		void OnEnemyDamage(const EnemyDamagePayload& payload) override;
		void OnEnemyDeath(const EnemyDeathPayload& payload) override;
		
	};
}
