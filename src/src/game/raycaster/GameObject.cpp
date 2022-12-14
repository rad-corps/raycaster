#include "GameObject.h"
#include "AI.h"
#include <iostream>
#include <algorithm>
#include <cassert>

namespace game
{
	GameObject::GameObject(rcgf::SpriteSheet* spritesheet, math::Transform transform, std::unique_ptr<AI> ai, std::unique_ptr<RenderingComponent> rc)
		: m_spritesheet{ spritesheet }
		, m_transform{ transform }
		, m_ai{ std::move(ai) }
		, m_rc{ std::move(rc) }
		, m_active{true}
	{}

	void GameObject::Update()
	{
		if (!m_active) return;

		std::unique_ptr<AI> newAI = m_ai->Update(*this);
		if (newAI)
		{
			m_ai = std::move(newAI);
		}
	}

	void GameObject::Render(const game::RenderEngine& re, const math::Transform& pov) const
	{
		if (!m_active) return;

		m_rc->Render(re, pov, m_transform, m_spritesheet);
	}

	GameObjectPool::GameObjectPool(size_t count, const game::RenderEngine& re)
		: m_gameObjects(count)
		, m_renderEngine{re}
	{}

	void GameObjectPool::Add(GameObject&& go)
	{
		// find next available
		int iterations = 0;
		while (m_gameObjects[++m_index].m_active)
		{
			++iterations;
			if (m_index == m_gameObjects.size() - 1)
			{
				m_index = -1;
			}
			assert(iterations < m_gameObjects.size());
		}
		std::cout << "took " << iterations << " tries to find an inactive gameobject in the pool. index: " << m_index << std::endl;
		m_gameObjects[m_index] = std::move(go);
	}

	void GameObjectPool::Update()
	{
		for (auto& go : m_gameObjects)
		{
			go.Update();
		}
	}

	void GameObjectPool::Render(const math::Transform& pov)
	{
		// sort before render
		const auto& playerPos = pov.pos;
		std::sort(m_gameObjects.begin(), m_gameObjects.end(), [playerPos](const GameObject& a, const GameObject& b) {
			return math::magnitude(playerPos - a.m_transform.pos) > math::magnitude(playerPos - b.m_transform.pos);
		});

		for (const auto& go : m_gameObjects)
		{
			go.Render(m_renderEngine, pov);
		}
	}

}

