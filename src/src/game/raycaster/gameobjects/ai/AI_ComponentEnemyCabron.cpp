#include "AI_ComponentEnemyCabron.h"
#include "./gameobjects/base/GameObject.h"
#include <iostream>
#include <iomanip> // std::setprecision
#include "EventSystem.h"
#include "Spatial.h" // game::line_of_sight
#include "Map.h"

namespace
{
	void calculate_path(const math::Vec2& startingPos, const math::Vec2& goalPos, const game::GameMap& gameMap)
	{
		// convert startingPos and goalPos into map indices
		//int startingPosIdx = game::toMapIndex(startingPos);
		//int goalPosIdx = game::toMapIndex(goalPos);
		auto adjacentMapIndices = game::getAdjacentMapIndices(0);

		for (auto idx : adjacentMapIndices)
		{
			std::cout << idx << std::endl;
		}



	}
}

namespace game
{
	std::unique_ptr<AI_Component> AI_WaypointFollow::Update(GameObject& subject, GameObjectPool& pool, const GameMap& gameMap, const std::vector<math::Transform>& playerTransforms)
	{
		// get current position of this npc
		const math::Vec2& currentPos = subject.m_transform.pos;

		// get next position of this npc
		const math::Vec2& nextPos = m_waypointPositions[m_waypointIndex];

		// wip: this will replace the dumb moving towards goal disregarding walls
		calculate_path(currentPos, nextPos, gameMap);

		// calculate direction to travel
		const math::Vec2 movementDelta = nextPos - currentPos;
		const math::Vec2 direction = math::normalize(movementDelta);
		subject.m_transform.angle = math::vec_to_angle(direction);
		const math::Vec2 velocity = direction * ACTOR_VELOCITY;

		//#FFFF00 yello
		Color color = { 0xFF, 0xFF, 0x0, 0xFF };

		// can we see the player?
		for (const math::Transform& playerTransform : playerTransforms)
		{			
			if (within_frustum(subject.m_transform, playerTransform.pos))
			{
				if (line_of_sight(subject.m_transform.pos, playerTransform.pos))
				{
					color = { 0xFF, 0x0, 0x0, 0xFF };
					subject.m_transform.pos += velocity * 0.5f;
				}
				else
				{
					subject.m_transform.pos += velocity;
				}
			}
			else
			{
				subject.m_transform.pos += velocity;
			}
			

			// check destination reached
			if (math::magnitude(movementDelta) < 1.f /* TODO: un-magic number this epsilon */)
			{
				m_waypointIndex == m_waypointPositions.size() - 1 ? m_waypointIndex = 0 : ++m_waypointIndex;
			}
			
			// line from enemy to collisionData sent to RenderEngine to draw top down map
			ColouredRect cr;
			cr.color = color;
			constexpr int sz = 6;
			cr.rect.x = subject.m_transform.pos.x - sz / 2;
			cr.rect.y = subject.m_transform.pos.y - sz / 2;
			cr.rect.w = sz;
			cr.rect.h = sz;
			events::publish(events::ColouredRectEvent{ cr });

			// draw line indicating enemy direction
			ColouredLine cl;
			cl.color = color;
			// get direction as a scaled vector
			const math::Vec2 forwardVec = math::angle_to_vec(subject.m_transform.angle);
			math::Vec2 scaledDir = math::scale(forwardVec, 10.f);
			cl.line.start = { subject.m_transform.pos.x, subject.m_transform.pos.y };
			cl.line.end = { subject.m_transform.pos.x + scaledDir.x, subject.m_transform.pos.y + scaledDir.y };
			events::publish(events::ColouredLineEvent{ cl });
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
