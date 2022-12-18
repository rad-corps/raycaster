#include "GameObjectPool.h"
#include <cassert>
#include <algorithm>
#include <iostream>

namespace game
{
	GameObjectPool::GameObjectPool(size_t count, const game::RenderEngine& re)
		: m_gameObjects(count)
		, m_renderEngine{ re }
	{}

	std::vector<GameObject>& GameObjectPool::GetPool()
	{
		return m_gameObjects;
	}

	void GameObjectPool::Add(GameObject&& go)
	{
		// easier to deal with int so we know we've checked all slots when we hit -1
		// don't want to deal with size_t underflow, and we are never going to have 
		// more than INT_MAX in our gameobject pool
		int index = static_cast<int>(m_gameObjects.size() - 1);
		int iterations = 0;
		while (m_gameObjects[index].m_active)
		{
			--index;
			++iterations;

			// TODO: if we can't find an object in the pool, reuse the furthest or something?
			assert(index >= 0);
		}
		m_gameObjects[index] = std::move(go);

		// keep track of the pool depth
		const int count = static_cast<int>(std::count_if(m_gameObjects.begin(), m_gameObjects.end(), [](const GameObject& go) {return go.m_active; }));
		if (count > m_maxActivePoolCount)
		{
			m_maxActivePoolCount = count;
			std::cout << "new depth for object pool. " << m_maxActivePoolCount << " active elements with " << GAME_OBJECT_ACTIVE_POOL_SZ << " total size" << std::endl;
		}
	}

	void GameObjectPool::Update(const game::GameMap& map)
	{
		for (auto& go : m_gameObjects)
		{
			go.Update(*this, map);
		}
	}

	void GameObjectPool::Render(const math::Transform& pov)
	{
		// sort before render
		const auto& playerPos = pov.pos;
		std::sort(m_gameObjects.begin(), m_gameObjects.end(), [playerPos](const GameObject& a, const GameObject& b) {
			// if both active, then sort by distance
			if (a.m_active && b.m_active)
				return math::magnitude(playerPos - a.m_transform.pos) > math::magnitude(playerPos - b.m_transform.pos);
			else if (a.m_active)
				return true;
			return false;
			});

		for (const auto& go : m_gameObjects)
		{
			go.Render(m_renderEngine, pov);
		}
	}
}
