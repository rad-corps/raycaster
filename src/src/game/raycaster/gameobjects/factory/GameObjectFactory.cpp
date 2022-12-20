#include "GameObjectFactory.h"
#include "SpriteSheet.h"
#include <memory>
#include "./gameobjects/render/RenderComponentCabron.h"
#include "./gameobjects/render/RenderComponentBullet.h"
#include "./gameobjects/render/RenderComponentBulletHitFX.h"
#include "./gameobjects/AI/AI_ComponentEnemyCabron.h"
#include "./gameobjects/AI/AI_ComponentBullet.h"


namespace
{
	std::unique_ptr<rcgf::SpriteSheet> ssEnemy01; // Cabron
	std::unique_ptr<rcgf::SpriteSheet> ssBullet;
	SDL_Renderer* m_renderer;
}

namespace game::factory
{
	void Init(SDL_Renderer* renderer)
	{
		m_renderer = renderer;
		{
			auto txEnemy01 = std::make_unique<rcgf::Texture>(renderer, "img/CabronTileset.png");
			ssEnemy01 = std::make_unique<rcgf::SpriteSheet>(std::move(txEnemy01), 64, 64, 8, 8);
		}

		{
			auto txBullet = std::make_unique<rcgf::Texture>(renderer, "./img/effects.png");
			ssBullet = std::make_unique<rcgf::SpriteSheet>(std::move(txBullet), 16, 16, 8, 8);
		}
	}

	GameObject CreateCabron(const math::Transform& transform)
	{
		auto ai = std::make_unique<AI_Empty>();
		auto rc = std::make_unique<RenderComponentCabron>();
		return GameObject{ ssEnemy01.get(), transform, std::move(ai), std::move(rc) };
	}

	GameObject CreateCabron(const math::Transform& transform, const std::vector<math::Vec2>& waypoints)
	{
		auto ai = std::make_unique<AI_WaypointFollow>(waypoints);
		auto rc = std::make_unique<RenderComponentCabron>();
		return GameObject{ ssEnemy01.get(), transform, std::move(ai), std::move(rc) };
	}

	GameObject CreatePlayerBullet(const math::Transform& transform)
	{
		auto ai = std::make_unique<AI_ComponentBullet>();
		auto rc = std::make_unique<RenderComponentBullet>();
		return GameObject{ ssBullet.get(), transform, std::move(ai), std::move(rc) };
	}

	GameObject CreatePlayerBulletHitFX(const math::Transform& transform)
	{
		auto rc = std::make_unique<RenderComponentBulletHitFX>();
		return GameObject{ ssBullet.get(), transform, nullptr, std::move(rc) };
	}
}