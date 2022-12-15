#include "GameObjectFactory.h"
#include "SpriteSheet.h"
#include <memory>
#include "./gameobjects/base/AI.h"
#include "./gameobjects/base/RenderingComponent.h"
#include "./gameobjects/render/CabronRenderer.h"
#include "./gameobjects/render/BulletRenderer.h"
#include "./gameobjects/AI/CabronAI.h"
#include "./gameobjects/AI/BulletAI.h"


namespace
{
	std::unique_ptr<rcgf::SpriteSheet> ssEnemy01; // Cabron
	std::unique_ptr<rcgf::SpriteSheet> ssBullet;
}

namespace game
{
	GameObjectFactory::GameObjectFactory(SDL_Renderer* renderer)
	{
		{
			auto txEnemy01 = std::make_unique<rcgf::Texture>(renderer, "img/CabronTileset.png");
			ssEnemy01 = std::make_unique<rcgf::SpriteSheet>(std::move(txEnemy01), 64, 64, 1, 4);
		}

		{
			auto txBullet = std::make_unique<rcgf::Texture>(renderer, "./img/player_bullet.png");
			ssBullet = std::make_unique<rcgf::SpriteSheet>(std::move(txBullet), 64, 64, 1, 1);
		}
	}

	GameObject GameObjectFactory::CreateCabron(const math::Transform& transform)
	{
		auto ai = std::make_unique<AI_Empty>();
		auto rc = std::make_unique<CabronRenderer>();
		return GameObject{ ssEnemy01.get(), transform, std::move(ai), std::move(rc) };
	}

	GameObject GameObjectFactory::CreatePlayerBullet(const math::Transform& transform)
	{
		auto ai = std::make_unique<BulletBehavior>();
		auto rc = std::make_unique<BulletRenderer>();
		return GameObject{ ssBullet.get(), transform, std::move(ai), std::move(rc) };
	}
}