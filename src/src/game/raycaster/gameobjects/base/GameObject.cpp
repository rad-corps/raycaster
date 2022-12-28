#include "GameObject.h"
#include "./gameobjects/base/AI_Component.h"
#include <iostream>

#include <cassert>

namespace game
{
	GameObject::GameObject(rcgf::SpriteSheet* spritesheet, math::Transform transform, std::unique_ptr<AI_Component> ai, std::unique_ptr<RenderComponent> rc)
		: m_spritesheet{ spritesheet }
		, m_transform{ transform }
		, m_ai{ std::move(ai) }
		, m_rc{ std::move(rc) }
		, m_active{true}
	{}

	void GameObject::SendEnemyDamaged(const EnemyDamagePayload& payload)
	{
		if (m_ai) m_ai->OnEnemyDamage(payload);
		if (m_rc) m_rc->OnEnemyDamage(payload);
	}

	void GameObject::SendEnemyDeath(const EnemyDeathPayload& payload)
	{
		if (m_ai) m_ai->OnEnemyDeath(payload);
		if (m_rc) m_rc->OnEnemyDeath(payload);
	}

	void GameObject::Update(GameObjectPool& gameObjects, const game::GameMap& map, const std::vector<math::Transform> playerTransforms)
	{
		if (!m_active || !m_ai) return;

		std::unique_ptr<AI_Component> newAI = m_ai->Update(*this, gameObjects, map, playerTransforms);
		if (newAI)
		{
			m_ai = std::move(newAI);
		}
	}

	void GameObject::Render(const game::RenderEngine& re, const math::Transform& pov, double deltatime) 
	{
		if (!m_active) return;

		m_rc->Render(re, pov, *this, m_spritesheet, deltatime);
	}
}

