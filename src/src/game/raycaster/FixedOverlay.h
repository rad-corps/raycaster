#pragma once

#include "Globals.h"
#include "RCGFMath.h"

namespace game::overlay
{
	enum class FireStatus
	{
		NOT_FIRING,
		READY_01,
		FIRE,
		RECOIL
	};

	void start_fire();
	void stop_fire();

	void init(SDL_Renderer* renderer);
	void render_weapon();
	void update_weapon(const math::Transform& playerTransform); // called every 1/60th of a second (16.6666ms)
}