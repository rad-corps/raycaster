#include "GameObjectFactory.h"
#include "SpriteSheet.h"
#include <memory>
#include "./gameobjects/base/GameObjectPool.h"
#include "./gameobjects/render/RenderComponentCabron.h"
#include "./gameobjects/render/RenderComponentBullet.h"
#include "./gameobjects/render/RenderComponentBulletHitFX.h"
#include "./gameobjects/AI/AI_ComponentEnemyCabron.h"
#include "./gameobjects/AI/AI_ComponentBullet.h"


namespace
{
	std::unique_ptr<rcgf::SpriteSheet> ssEnemy01; // Cabron
	std::unique_ptr<rcgf::SpriteSheet> ssBullet;
	std::unique_ptr<rcgf::SpriteSheet> ssWeapons;

	// WeaponsTileset.png
	SDL_Renderer* m_renderer;
	game::GameObjectPool* m_gameObjects;
}

namespace game::factory
{
	void Init(SDL_Renderer* renderer, GameObjectPool* gameObjects)
	{
		m_gameObjects = gameObjects;
		m_renderer = renderer;
		
		auto txEnemy01 = std::make_unique<rcgf::Texture>(renderer, "./img/CabronTileset.png");
		auto txBullet = std::make_unique<rcgf::Texture>(renderer, "./img/effects.png");
		auto txWeapons = std::make_unique<rcgf::Texture>(renderer, "./img/WeaponsTileset.png");

		ssEnemy01 = std::make_unique<rcgf::SpriteSheet>(std::move(txEnemy01), 64, 64, 8, 8);
		ssBullet = std::make_unique<rcgf::SpriteSheet>(std::move(txBullet), 16, 16, 8, 8);
		ssWeapons = std::make_unique<rcgf::SpriteSheet>(std::move(txWeapons), 64, 64, 7, 7);
	}

	void Clear()
	{
		m_gameObjects->Clear();
	}

	void CreateCabron(const math::Transform& transform, const std::vector<math::Vec2>& waypoints)
	{
		auto ai = std::make_unique<AI_WaypointFollow>(waypoints);
		auto rc = std::make_unique<RenderComponentCabron>();
		m_gameObjects->Add(GameObject{ ssEnemy01.get(), transform, std::move(ai), std::move(rc), "Cabron", true});
	}

	void CreatePlayerBullet(const math::Transform& transform, GameObject* origin)
	{
		auto ai = std::make_unique<AI_ComponentBullet>(origin);
		auto rc = std::make_unique<RenderComponentBullet>();
		m_gameObjects->Add(GameObject{ ssWeapons.get(), transform, std::move(ai), std::move(rc), "Bullet", false});
	}

	void CreatePlayerBulletHitFX(const math::Transform& transform)
	{
		auto rc = std::make_unique<RenderComponentBulletHitFX>();
		m_gameObjects->Add(GameObject{ ssBullet.get(), transform, nullptr, std::move(rc), "BulletHitFX", false});
	}
}