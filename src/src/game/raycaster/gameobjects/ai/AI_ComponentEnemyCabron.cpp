#include "AI_ComponentEnemyCabron.h"
#include "./gameobjects/base/GameObject.h"
#include "./gameobjects/base/GameObjectPool.h"
#include "./gameobjects/factory/GameObjectFactory.h"
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
		m_timer = 0.f;
		m_behaviour = Behaviour::PATROL;
	}
	
	void AI_WaypointFollow::InitEngage(GameObject& subject, const math::Vec2& engagePos)
	{
		std::cout << "Engage" << std::endl;
		m_waypointPositions = game::spatial::do_pathfinding(subject.m_transform.pos, engagePos);
		m_waypointIndex = 0;
		m_timer = 0.f;
		m_behaviour = Behaviour::ENGAGE;
	}

	void AI_WaypointFollow::InitDisengage(GameObject& subject)
	{
		std::cout << "Disengage" << std::endl;
		m_waypointPositions.clear();
		m_waypointIndex = 0;
		m_timer = 0.f;
		m_behaviour = Behaviour::DISENGAGE;
	}

	void AI_WaypointFollow::InitReturn(GameObject& subject)
	{
		std::cout << "Return" << std::endl;
		m_timer = 0.f;
		if (m_patrolWaypointPositions.size() == 0)
		{
			m_behaviour = Behaviour::PATROL;
			return;
		}

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
		// can we see the player?
		if (CanSeePlayer(subject, playerTransforms))
		{
			// todo: unhardcode playerTransforms[0]
			InitEngage(subject, playerTransforms[0].pos);
			return;
		}

		if (m_waypointPositions.size() == 0) return;
		
		const math::Vec2& currentPos = subject.m_transform.pos;
		const math::Vec2& nextPos = m_waypointPositions[m_waypointIndex];
		const math::Vec2 movementDelta = nextPos - currentPos;
		const math::Vec2 direction = math::normalize(movementDelta);
		const math::Vec2 velocity = direction * ACTOR_VELOCITY;

		subject.m_transform.angle = math::vec_to_angle(direction);
		subject.m_transform.pos += velocity;

		// check destination reached
		if (math::magnitude(movementDelta) < 1.f /* TODO: un-magic number this epsilon */)
		{
			m_waypointIndex == m_waypointPositions.size() - 1 ? m_waypointIndex = 0 : ++m_waypointIndex;
		}
	}
	void AI_WaypointFollow::DoEngage(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) 
	{
		m_timer += FrameTime;
		
		if (m_engageBehaviour == EngageBehaviour::WALKING)
		{
			const math::Vec2& currentPos = subject.m_transform.pos;
			const math::Vec2& nextPos = m_waypointPositions[m_waypointIndex];
			const math::Vec2 movementDelta = nextPos - currentPos;

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
				
				// return early, if movementDelta == 0 then below calculations will cause failure.
				return; 
			}


			const math::Vec2 direction = math::normalize(movementDelta);
			const math::Vec2 velocity = direction * ACTOR_VELOCITY;

			subject.m_transform.angle = math::vec_to_angle(direction);
			subject.m_transform.pos += velocity;

			if (m_timer > 0.50f)
			{
				m_timer = 0.f;

				if (spatial::line_of_sight(subject.m_transform.pos, playerTransforms[0].pos))
				{
					m_engageBehaviour = EngageBehaviour::GUN_DRAW;
					subject.SendAiAnimation(AiAnimation::GunDraw);
				}
			}

			
		}
		else if (m_engageBehaviour == EngageBehaviour::GUN_DRAW)
		{
			if (m_timer > 0.25f)
			{
				m_timer = 0.f;
				m_engageBehaviour = EngageBehaviour::GUN_AIM;
				subject.SendAiAnimation(AiAnimation::GunAim);
			}
		}
		else if (m_engageBehaviour == EngageBehaviour::GUN_AIM)
		{
			if (m_timer > 0.25f)
			{
				m_timer = 0.f;
				m_engageBehaviour = EngageBehaviour::GUN_FIRE;
				subject.SendAiAnimation(AiAnimation::GunFire);
			}
		}
		else if (m_engageBehaviour == EngageBehaviour::GUN_FIRE)
		{
			m_timer = 0.f;
			math::Transform bulletTransform = subject.m_transform;
			math::Vec2 enemyToPlayer = playerTransforms[0].pos - subject.m_transform.pos;
			bulletTransform.angle = math::vec_to_angle_pos(enemyToPlayer);
			game::factory::CreatePlayerBullet(bulletTransform, &subject);
			m_engageBehaviour = EngageBehaviour::GUN_FIRED;
		}
		else if (m_engageBehaviour == EngageBehaviour::GUN_FIRED)
		{
			// lets reuse the timer, little hacky, but simple
			if (m_timer > 0.1f && m_timer < 0.99f)
			{
				m_timer = 1.f;
				subject.SendAiAnimation(AiAnimation::GunAim);
			}
			else if (m_timer > 1.1f)
			{
				m_timer = 0.f;

				// todo: random chance wether we walk or fire again
				
				// between 0.f and 1.f
				if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) >= 0.5f)
				{
					m_engageBehaviour = EngageBehaviour::WALKING;
					subject.SendAiAnimation(AiAnimation::Walking);
				}
				else
				{
					m_engageBehaviour = EngageBehaviour::GUN_AIM;
					subject.SendAiAnimation(AiAnimation::GunAim);
				}
				
			}
		}

		// can we see the player? (TODO timeout engage on x seconds without line of sight)
		if (spatial::line_of_sight(subject.m_transform.pos, playerTransforms[0].pos))
		{

		}
	}
	void AI_WaypointFollow::DoDisengage(GameObject& subject, GameObjectPool& gameObjects, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms) 
	{
		m_timer += FrameTime;
		if (spatial::line_of_sight(subject.m_transform.pos, playerTransforms[0].pos))
		{
			InitEngage(subject, playerTransforms[0].pos);
		}
		else if (m_timer > 3.f)
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
			if (m_waypointIndex == m_waypointPositions.size() - 1)
			{
				// init patrol
				InitPatrol(subject);
			}
			++m_waypointIndex;
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
		if (m_behaviour == Behaviour::DEAD) return nullptr;

		switch (m_behaviour)
		{
			case Behaviour::PATROL   : DoPatrol(subject, pool, gameMap, playerTransforms); break;
			case Behaviour::ENGAGE   : DoEngage(subject, pool, gameMap, playerTransforms); break;
			case Behaviour::DISENGAGE: DoDisengage(subject, pool, gameMap, playerTransforms); break;
			case Behaviour::RETURN   : DoReturn(subject, pool, gameMap, playerTransforms); break;
		}

		DrawDebugWaypoints();
		return nullptr;
	}

	void AI_WaypointFollow::SetWaypointPositions(std::vector<math::Vec2>&& waypointPositions)
	{
		m_waypointIndex = 0;
		m_waypointPositions = waypointPositions;
	}

	void AI_WaypointFollow::OnEnemyDamage(const EnemyDamagePayload& payload)
	{
		m_health -= 1.f;
		if (m_health <= 0.f)
		{
			// TODO: send event to GameObject
			payload.gameObject->SendEnemyDeath();
			payload.gameObject->m_collidable = false;
			m_behaviour = Behaviour::DEAD;
		}
	}

	void AI_WaypointFollow::OnAlert(const math::Vec2& pos, const math::Vec2& alertPos)
	{
		m_waypointPositions = game::spatial::do_pathfinding(pos, alertPos);
		m_waypointIndex = 0;
	}
}
