#include "AI_ComponentBullet.h"
#include "./gameobjects/base/GameObject.h"
#include "RaycasterConstants.h"

namespace game
{
	std::unique_ptr<AI_Component> AI_ComponentBullet::Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap)
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
				gameObject.SendEnemyDamaged(EnemyDamagePayload{ 1.f });
				gameObject.m_active = false;
			}
		}

		return nullptr;
	}
}