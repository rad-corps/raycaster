#include "GameObject.h"
#include "./gameobjects/base/AI_Component.h"
#include <iostream>

#include <cassert>

namespace game
{
	GameObject::GameObject(rcgf::SpriteSheet* spritesheet, math::Transform transform, std::unique_ptr<AI_Component> ai, std::unique_ptr<RenderComponent> rc, const std::string& name, bool collidable)
		: m_spritesheet{ spritesheet }
		, m_transform{ transform }
		, m_ai{ std::move(ai) }
		, m_rc{ std::move(rc) }
		, m_active{true}
		, m_name{name}
		, m_collidable{collidable}
	{
	}

	void GameObject::SendEnemyDamaged(const EnemyDamagePayload& payload)
	{
		if (m_ai) m_ai->OnEnemyDamage(payload);
		if (m_rc) m_rc->OnEnemyDamage(payload);
	}

	void GameObject::SendEnemyDeath()
	{
		if (m_ai) m_ai->OnEnemyDeath();
		if (m_rc) m_rc->OnEnemyDeath();
	}

	void GameObject::SendAlert(const math::Vec2& alertPos)
	{
		if (m_ai) m_ai->OnAlert(m_transform.pos, alertPos);
		if (m_rc) m_rc->OnAlert(m_transform.pos, alertPos);
	}
	
	void GameObject::SendAiAnimation(AiAnimation aiAnimation)
	{
		if (m_ai) m_ai->OnAnimationUpdate(aiAnimation);
		if (m_rc) m_rc->OnAnimationUpdate(aiAnimation);
	}

	void GameObject::Update(GameObjectPool& gameObjects, const map::GameMap& map, const std::vector<math::Transform> playerTransforms)
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

