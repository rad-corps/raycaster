#pragma once

#include <vector>
#include "RenderEngine.h"
#include "RCGFMath.h"
#include "GameObject.h"

namespace game
{
	class GameObjectPool
	{
	public:
		// todo make this a template and std::array
		GameObjectPool(size_t num, const game::RenderEngine& re);

		// GameObject is move only (no copy construction)
		void Add(GameObject&& go);
		void Update(const game::GameMap& map);
		void Render(const math::Transform& pov);
		std::vector<GameObject>& GetPool();

	private:
		std::vector<GameObject> m_gameObjects;
		const RenderEngine& m_renderEngine;

		// TODO: For debugging info. Report whenever the pool hits its highest point.
		int m_maxActivePoolCount = 0;
	};
}