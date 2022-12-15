#include "AI.h"
#include "GameObject.h"
#include "RaycasterConstants.h"
#include <iostream>

namespace game
{
	std::unique_ptr<AI> AI_WaypointFollow::Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap)
	{
		// get current position
		const math::Vec2& currentPos = subject.m_transform.pos;

		// get next position
		const math::Vec2& nextPos = m_waypointPositions[m_waypointIndex];

		// calculate direction to travel
		const math::Vec2 delta = nextPos - currentPos;
		const math::Vec2 direction = math::normalize(delta);
		const math::Vec2 velocity = direction * ACTOR_VELOCITY;

		// travel
		subject.m_transform.pos += velocity;
		subject.m_transform.angle = math::vec_to_angle(direction);

		// check destination reached
		if (math::magnitude(delta) < 0.5f /* TODO: un-magic number this epsilon */ )
		{
			m_waypointIndex == m_waypointPositions.size() - 1 ? m_waypointIndex = 0 : ++m_waypointIndex;
		}

		return nullptr;
	}

	std::unique_ptr<AI> AI_Empty::Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap)
	{
		return nullptr;
	}

	std::unique_ptr<AI> BulletBehavior::Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap)
	{
		math::Vec2 direction = math::angle_to_vec(subject.m_transform.angle);
		math::Vec2 velocity = direction * 1;
		subject.m_transform.pos += velocity;

		// check for collision with map
		if (isWall((int)subject.m_transform.pos.x, (int)subject.m_transform.pos.y, &gameMap))
		{
			// we hit a wall. deactivate
			subject.m_active = false;
			return nullptr;
		}
		
		// compare against enemies
		for (auto& gameObject : gameObjects)
		{
			if (!gameObject.m_active)
				continue;

			// are we looking at ourselves? disregard!
			if (&subject == &gameObject)
				continue;

			// get the distance from subject to gameObject
			if (math::magnitude(gameObject.m_transform.pos - subject.m_transform.pos) < 3.f /* TODO: this shouldn't be a magic number*/)
			{
				// todo: damage enemy
				gameObject.m_active = false;
				subject.m_active = false;
			}
		}

		return nullptr;
	}
}