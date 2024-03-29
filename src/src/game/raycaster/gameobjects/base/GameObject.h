#pragma once

#include <memory>
#include "SpriteSheet.h"
#include "./gameobjects/base/AI_Component.h"
#include "RCGFMath.h"
#include "./gameobjects/base/RenderComponent.h"
#include "RenderEngine.h"

namespace game
{
	class GameObjectPool;

	// create one of these per object in world
	class GameObject
	{
	public:
		// default construction
		GameObject()
			: m_transform{ 0.f,0.f,0.f }
			, m_active{false}
			, m_name{"empty"}
		{}
		
		// remove copy construction
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		
		// allow move construction
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		GameObject(rcgf::SpriteSheet* spritesheet, math::Transform transform, std::unique_ptr<AI_Component> ai, std::unique_ptr<RenderComponent> rc, const std::string& name, bool collidable);

		void Update(GameObjectPool& gameObjects, const map::GameMap& map, const std::vector<math::Transform> playerTransforms); // fixed update (60 per second)
		void Render(const game::RenderEngine& re, const math::Transform& pov, double deltatime);

		// send events
		// TODO: event sending really needs to be generic, this is not fun to maintain
		void SendEnemyDamaged(const EnemyDamagePayload& payload);
		void SendEnemyDeath();
		void SendAlert(const math::Vec2& alertPos);
		void SendAiAnimation(AiAnimation aiAnimation);
		
		math::Transform m_transform;

		// todo make this private and make GameObjectPool a friend
		bool m_active = false;
		bool m_collidable = false;

	private:
		
		rcgf::SpriteSheet* m_spritesheet;
		std::unique_ptr<AI_Component> m_ai;
		std::unique_ptr<RenderComponent> m_rc;
		std::string m_name;
	};
} 