#include "CabronAI.h"
#include "./gameobjects/base/GameObject.h"

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
		if (math::magnitude(delta) < 0.5f /* TODO: un-magic number this epsilon */)
		{
			m_waypointIndex == m_waypointPositions.size() - 1 ? m_waypointIndex = 0 : ++m_waypointIndex;
		}

		return nullptr;
	}

	std::unique_ptr<AI> AI_Empty::Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap)
	{
		return nullptr;
	}
}
