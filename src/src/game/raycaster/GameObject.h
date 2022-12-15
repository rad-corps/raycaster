#pragma once

#include <memory>
#include "SpriteSheet.h"
#include "AI.h"
#include "RCGFMath.h"
#include "RenderingComponent.h"
#include "RenderEngine.h"

namespace game
{
	// create one of these per object in world
	class GameObject
	{
	public:
		// default construction
		GameObject()
			: m_transform{ 0.f,0.f,0.f }
			, m_active{false}
		{}
		
		// remove copy construction
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		
		// allow move construction
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		GameObject(rcgf::SpriteSheet* spritesheet, math::Transform transform, std::unique_ptr<AI> ai, std::unique_ptr<RenderingComponent> rc);

		void Update(std::vector<GameObject>& gameObjects, const game::GameMap& map);
		void Render(const game::RenderEngine& re, const math::Transform& pov) const;

		math::Transform m_transform;

		// todo make this private and make GameObjectPool a friend
		bool m_active = false;

	private:
		
		rcgf::SpriteSheet* m_spritesheet;
		std::unique_ptr<AI> m_ai;
		std::unique_ptr<RenderingComponent> m_rc;
	};

	class GameObjectPool
	{
	public:
		// todo make this a template and std::array
		GameObjectPool(size_t num, const game::RenderEngine& re);
		
		// GameObject is move only (no copy construction)
		void Add(GameObject&& go);
		void Update(const game::GameMap& map);
		void Render(const math::Transform& pov);

	private:
		std::vector<GameObject> m_gameObjects;
		const RenderEngine& m_renderEngine;

		// TODO: For debugging info. Report whenever the pool hits its highest point.
		int m_maxActivePoolCount = 0;
	};
} 