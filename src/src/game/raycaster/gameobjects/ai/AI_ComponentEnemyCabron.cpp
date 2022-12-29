#include "AI_ComponentEnemyCabron.h"
#include "./gameobjects/base/GameObject.h"
#include <iostream>
#include "EventSystem.h"

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
			float enemyToPlayerAngle = math::vec_to_angle_pos(enemyToPlayerDir);

			// get distance to wall from subject position using direction
			const RayWallCollision collisionData = FindWallHitPos(subject.m_transform, enemyToPlayerAngle, &gameMap);
			
			const float enemyToPlayerDist = math::magnitude(enemyToPlayerDelta);

			if (collisionData.distance > enemyToPlayerDist)
			{
				//std::cout << "can see player!" << std::endl;
			}
			else
			{
				//std::cout << "can not see player!" << std::endl;
			}
			
			
			{
				// line from enemy to collisionData sent to RenderEngine to draw top down map
				Color color = { 255,0,0,255 };
				ColouredLine l;
				l.line.start.x = subject.m_transform.pos.x;
				l.line.start.y = subject.m_transform.pos.y;
				l.line.end.x = collisionData.xHitPos;
				l.line.end.y = collisionData.yHitPos;
				l.color = color;
				events::publish(events::ColouredLineEvent{ l });

				// line from enemy to collisionData sent to RenderEngine to draw top down map
				ColouredRect cr;
				cr.color = color;
				constexpr int sz = 6;
				cr.rect.x = l.line.start.x - sz / 2;
				cr.rect.y = l.line.start.y - sz / 2;
				cr.rect.w = sz;
				cr.rect.h = sz;
				events::publish(events::ColouredRectEvent{ cr });
			}
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
