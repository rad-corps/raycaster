#pragma once

#include "gameobjects/base/AI_Component.h"
#include <cassert>

namespace game
{
	class AI_WaypointFollow : public AI_Component
	{
	public:
		AI_WaypointFollow() = delete;
		AI_WaypointFollow(std::vector<math::Vec2> waypointPositions)
			: m_waypointPositions{ waypointPositions }
			, m_patrolWaypointPositions{ waypointPositions }
		{}

		std::unique_ptr<AI_Component> Update(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) override;
		void OnEnemyDamage(const EnemyDamagePayload& payload) override;
		void OnAlert(const math::Vec2& pos, const math::Vec2& alertPos) override;

		float m_timer = 0.f;

	private:
		enum class Behaviour
		{
			PATROL,
			ENGAGE,
			DISENGAGE,
			RETURN,
			DEAD
		};

		enum class EngageBehaviour
		{
			WALKING,
			GUN_DRAW,
			GUN_AIM,
			GUN_FIRE,
			GUN_FIRED // Might be identical to AIM, but logically needed to escape firing cycle
		};
		
		Behaviour m_behaviour = Behaviour::PATROL;
		EngageBehaviour m_engageBehaviour = EngageBehaviour::WALKING;


		// TODO: This should be factored to somewhere shared by all AI
		bool CanSeePlayer(GameObject& subject, const std::vector<math::Transform>& playerTransforms);

		void InitPatrol(GameObject& subject);
		void InitEngage(GameObject& subject, const math::Vec2& engagePos);
		void InitDisengage(GameObject& subject);
		void InitReturn(GameObject& subject);

		void DoPatrol(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms);
		void DoEngage(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms);
		void DoDisengage(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms);
		void DoReturn(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms);

		void SetWaypointPositions(std::vector<math::Vec2>&& waypointPositions);
		void DrawDebugWaypoints();
		
		
		std::vector<math::Vec2> m_waypointPositions;
		int m_waypointIndex = 0;

		// need to cache these so we can return to them later
		std::vector<math::Vec2> m_patrolWaypointPositions;
		int m_waypointIndexCache = 0; // required for returning to initial path
		
		float m_health = 5.5f;
	};
}
