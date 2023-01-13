#include "AI_ComponentEnemyCabron.h"
#include "./gameobjects/base/GameObject.h"
#include <iostream>
#include <iomanip> // std::setprecision
#include "EventSystem.h"
#include "Spatial.h" // game::line_of_sight
#include "Map.h"

namespace
{
	constexpr float FrameTime = 0.01666f;
}

namespace game
{
	void AI_WaypointFollow::InitPatrol(GameObject& subject)
	{
		std::cout << "Patrol" << std::endl;
		m_waypointPositions = m_patrolWaypointPositions;
		m_waypointIndex = m_waypointIndexCache;
		m_behaviour = Behaviour::PATROL;
	}
	
	void AI_WaypointFollow::InitEngage(GameObject& subject, const math::Vec2& engagePos)
	{
		std::cout << "Engage" << std::endl;
		m_waypointPositions = game::spatial::do_pathfinding(subject.m_transform.pos, engagePos);
		m_waypointIndex = 0;
		m_firetimer = 0.f;
		m_behaviour = Behaviour::ENGAGE;
	}

	void AI_WaypointFollow::InitDisengage(GameObject& subject)
	{
		std::cout << "Disengage" << std::endl;
		m_waypointPositions.clear();
		m_waypointIndex = 0;
		m_disengagetimer = 0.f;
		m_behaviour = Behaviour::DISENGAGE;
	}

	void AI_WaypointFollow::InitReturn(GameObject& subject)
	{
		std::cout << "Return" << std::endl;

		// find the closest waypoint 
		float dist = 1000000.f;
		for (int i = 0; i < m_patrolWaypointPositions.size(); ++i)
		{
			const float compareDist = math::distance(m_patrolWaypointPositions[i], subject.m_transform.pos);
			if (compareDist < dist)
			{
				dist = compareDist;
				m_waypointIndexCache = i;
			}
		}

		m_waypointPositions = game::spatial::do_pathfinding(subject.m_transform.pos, m_patrolWaypointPositions[m_waypointIndexCache]);
		m_waypointIndex = 0;
		m_behaviour = Behaviour::RETURN;
	}

	void AI_WaypointFollow::DoPatrol(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) 
	{
		const math::Vec2& currentPos = subject.m_transform.pos;
		const math::Vec2& nextPos = m_waypointPositions[m_waypointIndex];
		const math::Vec2 movementDelta = nextPos - currentPos;
		const math::Vec2 direction = math::normalize(movementDelta);
		const math::Vec2 velocity = direction * ACTOR_VELOCITY;

		subject.m_transform.angle = math::vec_to_angle(direction);
		subject.m_transform.pos += velocity;

		// can we see the player?
		if (CanSeePlayer(subject, playerTransforms))
		{
			// todo: unhardcode playerTransforms[0]
			InitEngage(subject, playerTransforms[0].pos);
		}

		// check destination reached
		if (math::magnitude(movementDelta) < 1.f /* TODO: un-magic number this epsilon */)
		{
			m_waypointIndex == m_waypointPositions.size() - 1 ? m_waypointIndex = 0 : ++m_waypointIndex;
		}
		
	}
	void AI_WaypointFollow::DoEngage(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) 
	{
		m_firetimer += FrameTime;
		const math::Vec2& currentPos = subject.m_transform.pos;
		const math::Vec2& nextPos = m_waypointPositions[m_waypointIndex];
		const math::Vec2 movementDelta = nextPos - currentPos;
		const math::Vec2 direction = math::normalize(movementDelta);
		const math::Vec2 velocity = direction * ACTOR_VELOCITY;

		subject.m_transform.angle = math::vec_to_angle(direction);
		subject.m_transform.pos += velocity;

		// can we see the player?
		if (CanSeePlayer(subject, playerTransforms))
		{
			// todo: fix m_timer magic number
			// only recalculate once every five seconds
			if (m_firetimer > 1.f)
			{
				m_firetimer = 0.f;
				// todo: unhardcode playerTransforms[0]
				std::cout << "fire" << std::endl;
			}
		}

		// check destination reached
		if (math::magnitude(movementDelta) < 1.f /* TODO: un-magic number this epsilon */)
		{
			// we have reached the end of our path. 
			if (m_waypointIndex == m_waypointPositions.size() - 1)
			{
				// if we can see the player. RE-ENGAGE
				if (CanSeePlayer(subject, playerTransforms))
				{
					InitEngage(subject, playerTransforms[0].pos);
				}
				else
				{
					InitDisengage(subject);
				}
			}
			
			// next waypoint, or cycle if at end
			m_waypointIndex == m_waypointPositions.size() - 1 ? m_waypointIndex = 0 : ++m_waypointIndex;
		}
	}
	void AI_WaypointFollow::DoDisengage(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) 
	{
		m_disengagetimer += FrameTime;
		if (m_disengagetimer > 3.f)
		{
			InitReturn(subject);
		}
	}
	void AI_WaypointFollow::DoReturn(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) 
	{
		const math::Vec2& currentPos = subject.m_transform.pos;
		const math::Vec2& nextPos = m_waypointPositions[m_waypointIndex];
		const math::Vec2 movementDelta = nextPos - currentPos;
		const math::Vec2 direction = math::normalize(movementDelta);
		const math::Vec2 velocity = direction * ACTOR_VELOCITY;

		subject.m_transform.angle = math::vec_to_angle(direction);
		subject.m_transform.pos += velocity;

		// can we see the player?
		if (CanSeePlayer(subject, playerTransforms))
		{
			// todo: unhardcode playerTransforms[0]
			InitEngage(subject, playerTransforms[0].pos);
		}

		// check destination reached
		if (math::magnitude(movementDelta) < 1.f /* TODO: un-magic number this epsilon */)
		{
			++m_waypointIndex;
			if (m_waypointIndex == m_waypointPositions.size() - 1)
			{
				// init patrol
				InitPatrol(subject);
			}
		}
	}

	void AI_WaypointFollow::DrawDebugWaypoints()
	{
		// draw debug waypoints
		const math::Vec2* prevWaypoint = nullptr;
		for (const math::Vec2& waypoint : m_waypointPositions)
		{
			if (prevWaypoint != nullptr)
			{
				// draw a line from prevWaypoint to this waypoint
				events::publish(events::ColouredLineEvent(
					ColouredLine(prevWaypoint->x, prevWaypoint->y, waypoint.x, waypoint.y, Color{ 0xFF, 0xFF, 0xFF, 0xFF })
				));
			}

			ColouredRect cr;
			cr.color = Color{ 0xFF,0xFF,0xFF,0xFF };
			cr.rect.w = 2;
			cr.rect.h = 2;
			cr.rect.x = waypoint.x - 1;
			cr.rect.y = waypoint.y - 1;
			events::publish(events::ColouredRectEvent{ cr });

			prevWaypoint = &waypoint;
		}
	}

	bool AI_WaypointFollow::CanSeePlayer(GameObject& subject, const std::vector<math::Transform>& playerTransforms)
	{
		for (const math::Transform& playerTransform : playerTransforms)
		{
			if (spatial::within_frustum(subject.m_transform, playerTransform.pos))
			{
				if (spatial::line_of_sight(subject.m_transform.pos, playerTransform.pos))
				{
					return true;
				}
			}
		}
		return false;
	}


	std::unique_ptr<AI_Component> AI_WaypointFollow::Update(GameObject& subject, GameObjectPool& pool, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms)
	{
		DrawDebugWaypoints();

		switch (m_behaviour)
		{
		case Behaviour::PATROL:
			DoPatrol(subject, pool, gameMap, playerTransforms);
			break;
		case Behaviour::ENGAGE:
			DoEngage(subject, pool, gameMap, playerTransforms);
			break;
		case Behaviour::DISENGAGE:
			DoDisengage(subject, pool, gameMap, playerTransforms);
			break;
		case Behaviour::RETURN:
			DoReturn(subject, pool, gameMap, playerTransforms);
			break;
		}

		return nullptr;
	}

	void AI_WaypointFollow::SetWaypointPositions(std::vector<math::Vec2>&& waypointPositions)
	{
		m_waypointIndex = 0;
		m_waypointPositions = waypointPositions;
	}

	void AI_WaypointFollow::OnAlert(const math::Vec2& pos, const math::Vec2& alertPos)
	{
		m_waypointPositions = game::spatial::do_pathfinding(pos, alertPos);
		m_waypointIndex = 0;
	}

	std::unique_ptr<AI_Component> AI_Empty::Update(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms)
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
