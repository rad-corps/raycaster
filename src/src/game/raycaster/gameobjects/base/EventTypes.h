#pragma once

namespace game
{
	struct EnemyDamagePayload
	{
		float damage;
	};

	struct EnemyDeathPayload
	{

	};

	enum class AiAnimation
	{
		Walking,
		GunDraw,
		GunAim,
		GunFire,
		Hit,
		Death
	};
}