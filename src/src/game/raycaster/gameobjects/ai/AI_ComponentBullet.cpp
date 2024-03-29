#include "AI_ComponentBullet.h"
#include "./gameobjects/base/GameObject.h"
#include "./gameobjects/base/GameObjectPool.h"
#include "RaycasterConstants.h"
#include "./gameobjects/factory/GameObjectFactory.h"
#include "../config/DesignerConstants.h"

namespace
{
	constexpr float BULLET_SPEED = 3.f;
}

namespace game
{
	std::unique_ptr<AI_Component> AI_ComponentBullet::Update(GameObject& subject, GameObjectPool& pool, const map::GameMap& gameMap, const std::vector<math::Transform>& playerTransforms)
	{
		math::Vec2 direction = math::angle_to_vec(subject.m_transform.angle);
		math::Vec2 velocity = direction * BULLET_SPEED;
		subject.m_transform.pos += velocity;

		// check for collision with map
		if (map::is_wall((int)subject.m_transform.pos.x, (int)subject.m_transform.pos.y))
		{
			// march it back the velocity delta so its not inside the wall
			math::Transform fxTransform = subject.m_transform;
			fxTransform.pos -= velocity;

			// create a bullet hit FX
			factory::CreatePlayerBulletHitFX(fxTransform);
			
			// we hit a wall. deactivate
			subject.m_active = false;

			return nullptr;
		}

		// compare against enemies
		for (auto& gameObject : pool.GetPool())
		{
			if (!gameObject.m_active || !gameObject.m_collidable)
				continue;

			// dont let the bullet collide with itself and dont collide the bullet with the GameObject that shot the bullet 
			if (&subject == &gameObject || &gameObject == origin)
				continue;

			// get the distance from subject to gameObject
			if (math::magnitude(gameObject.m_transform.pos - subject.m_transform.pos) < 3.f /* TODO: this shouldn't be a magic number*/)
			{
				gameObject.SendEnemyDamaged(EnemyDamagePayload{ design::UZI_BULLET_DAMAGE, &gameObject });
				subject.m_active = false;
			}
		}

		return nullptr;
	}
}