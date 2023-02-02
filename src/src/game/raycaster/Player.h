#pragma once

#include "Globals.h"
#include "Map.h"
#include "RaycasterConstants.h"
#include "RCGFMath.h"

namespace game
{
	struct PlayerBullet
	{
		PlayerBullet(math::Transform initialPosition)
			: transform{ initialPosition }
			, velocity{0.f,0.f}
		{
			velocity = math::angle_to_vec(initialPosition.angle) * bulletSpeed;
		}
		void update()
		{
			transform.pos += velocity;
		}
		math::Transform transform;
		math::Vec2 velocity;
		static constexpr float bulletSpeed = 5.f;
	};

	class Player
	{
	public:
		Player(const math::Transform& startingPos);
		Player() = delete;

		enum class RotateDirection
		{
			Clockwise,
			Anticlockwise
		};

		math::Vec2 getOffset(float angle, float distance) const;
		void rotate(RotateDirection dir);
		void move(float relativeAngle, const map::GameMap* map);
		void setTransform(const math::Transform& transform);
		void fire();

		math::Transform transform;
		SDL_Rect wallCollisionBox;
	};
}