#pragma once

namespace game
{
	class GameObject;

	struct EnemyDamagePayload
	{
		float damage;
		GameObject* gameObject;
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