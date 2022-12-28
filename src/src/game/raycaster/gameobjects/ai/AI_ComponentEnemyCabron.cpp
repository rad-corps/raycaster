#include "AI_ComponentEnemyCabron.h"
#include "./gameobjects/base/GameObject.h"
#include <iostream>

namespace game
{
	std::unique_ptr<AI_Component> AI_WaypointFollow::Update(GameObject& subject, GameObjectPool& pool, const GameMap& gameMap, const std::vector<math::Transform>& playerTransforms)
	{
		// get current position of this npc
		const math::Vec2& currentPos = subject.m_transform.pos;

		// get next position of this npc
		const math::Vec2& nextPos = m_waypointPositions[m_waypointIndex];

		// calculate direction to travel
		const math::Vec2 delta = nextPos - currentPos;
		const math::Vec2 direction = math::normalize(delta);
		const math::Vec2 velocity = direction * ACTOR_VELOCITY;

		// travel
		subject.m_transform.pos += velocity;
		subject.m_transform.angle = math::vec_to_angle(direction);

		// check destination reached
		if (math::magnitude(delta) < 1.f /* TODO: un-magic number this epsilon */)
		{
			m_waypointIndex == m_waypointPositions.size() - 1 ? m_waypointIndex = 0 : ++m_waypointIndex;
		}


		// can we see the player?
		for (const math::Transform& playerTransform : playerTransforms)
		{
			// get direction from subject to playerTransform.pos
			const math::Vec2 enemyToPlayerDelta = playerTransform.pos - subject.m_transform.pos;
			const math::Vec2 enemyToPlayerDir = math::normalize(enemyToPlayerDelta);
			

			// get distance to wall from subject position using direction
			const RayWallCollision collisionData = FindWallHitPos(subject.m_transform, math::vec_to_angle(enemyToPlayerDir), &gameMap);


			// get distance from subject to playerTransform.pos
			const float enemyToPlayerDist = math::magnitude(enemyToPlayerDelta);

			// if wall dist > dist to player
				// can see!
			// else
				// can not see
			if (collisionData.distance > enemyToPlayerDist)
			{
				std::cout << "can see player!" << std::endl;
			}
			else
			{
				std::cout << "can not see player!" << std::endl;
			}
			Line l;
			l.start.x = (int)subject.m_transform.pos.x;
			l.start.y = (int)subject.m_transform.pos.y;
			l.end.x = (int)collisionData.xHitPos;
			l.end.y = (int)collisionData.yHitPos;
			subject.SendLineDraw(l);
		}


		return nullptr;
	}

	std::unique_ptr<AI_Component> AI_Empty::Update(GameObject& subject, GameObjectPool& gameObjects, const GameMap& gameMap, const std::vector<math::Transform>& playerTransforms)
	{
		return nullptr;
	}

	void AI_Empty::OnEnemyDamage(const EnemyDamagePayload& payload)
	{
		std::cout << "AI_Empty::OnEnemyDamage: " << payload.damage << std::endl;
	}

	void AI_Empty::OnEnemyDeath(const EnemyDeathPayload& payload)
	{
		std::cout << "AI_Empty::OnEnemyDeath" << std::endl;
	}
}
