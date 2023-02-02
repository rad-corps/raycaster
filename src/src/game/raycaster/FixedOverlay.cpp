#include "FixedOverlay.h"
#include "SpriteSheet.h"
#include <memory>
#include "gameobjects/factory/GameObjectFactory.h"

namespace
{
	std::unique_ptr<rcgf::SpriteSheet> ssPlayerWeapons;
	SDL_Renderer* renderer = nullptr;
	float msFireTimer = 0.f;
	float msWeaponSpeed = 200.f;
	float msWeaponReadySpeed = 50.f;
	game::overlay::FireStatus fireStatus = game::overlay::FireStatus::NOT_FIRING;
}

namespace game::overlay
{
	void init(SDL_Renderer* renderer_)
	{
		renderer = renderer_;
		auto txWeapons = std::make_unique<rcgf::Texture>(renderer, "./img/WeaponsTileset.png");
		ssPlayerWeapons = std::make_unique<rcgf::SpriteSheet>(std::move(txWeapons), 64, 64, 7, 7);
	}

	void render_weapon()
	{
		// TODO: this will need to change when we have split screen
		constexpr int sz = 64 * 8;
		constexpr int xPos = SCREEN_WIDTH / 2 - sz / 2;
		constexpr int yPos = SCREEN_HEIGHT - sz;
		SDL_Rect dstRect{ xPos, yPos, sz, sz};

		int weaponSpriteIndex = 0;

		switch (fireStatus)
		{
		case FireStatus::READY_01:
			weaponSpriteIndex = 1;
			break;
		case FireStatus::FIRE:
			weaponSpriteIndex = 2;
			break;
		case FireStatus::RECOIL:
			weaponSpriteIndex = 3;
			break;
		}

		ssPlayerWeapons->render(weaponSpriteIndex, &dstRect);
	}

	void update_weapon(const math::Transform& playerTransform)
	{
		if (fireStatus != FireStatus::NOT_FIRING) msFireTimer += 16.66666f;

		switch (fireStatus)
		{
		case FireStatus::READY_01:
			if (msFireTimer > msWeaponReadySpeed)
			{
				fireStatus = FireStatus::FIRE;
				msFireTimer = 0.f;
				factory::CreatePlayerBullet(playerTransform, nullptr);
			}
			break;
		case FireStatus::FIRE:
			if (msFireTimer > msWeaponSpeed / 2)
			{
				fireStatus = FireStatus::RECOIL;
				msFireTimer = 0.f;
			}
			break;
		case FireStatus::RECOIL:
			if (msFireTimer > msWeaponSpeed / 2)
			{
				fireStatus = FireStatus::FIRE;
				msFireTimer = 0.f;
				factory::CreatePlayerBullet(playerTransform, nullptr);
			}
			break;
		}
	}

	void start_fire()
	{
		if (fireStatus != FireStatus::NOT_FIRING)
			return;

		fireStatus = FireStatus::READY_01;
		msFireTimer = 0.f;
	}

	void stop_fire()
	{
		fireStatus = FireStatus::NOT_FIRING;
	}
}