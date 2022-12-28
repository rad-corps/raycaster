#include "RenderComponentCabron.h"
#include "RCGFMath.h"
#include <cassert>
#include "gameobjects/base/GameObject.h"
#include <iostream>

using math::PI;

namespace
{
	constexpr int SPRITESHEET_ROWS = 8;
	constexpr int SPRITESHEET_COLS = 8;
	constexpr double WALK_SPEED_MS = 350.0;
}

namespace game
{
	// column = rotation
	int RenderComponentCabron::CalculateSpriteAnimationIDColumn(float angle)
	{
		// 0: front, 1: right, 2: back, 3: left

		if (angle >= 7 * PI / 4 || angle <= PI / 4)
		{
			return 2;
		}
		if (angle >= PI / 4 && angle <= 3 * PI / 4)
		{
			return 3;
		}
		if (angle >= 3 * PI / 4 && angle <= 5 * PI / 4)
		{
			return 0;
		}
		if (angle >= 5 * PI / 4 && angle <= 7 * PI / 4)
		{
			return 1;
		}

		assert(false);
		return -1;
	}

	// row = walk cycle animation
	int RenderComponentCabron::CalculateSpriteAnimationIDRow()
	{
		return 1;
	}

	void RenderComponentCabron::Render(const RenderEngine& re, const math::Transform& pov, GameObject& gameObject, rcgf::SpriteSheet* spriteSheet, double deltatime)
	{
		animationTimer += deltatime;
		if (animationTimer > WALK_SPEED_MS)
		{
			animationTimer = 0.0;
			walkAnimationID = ++walkAnimationID > 4 ? 1 : walkAnimationID;
		}
		const math::Vec2 povToSprite = gameObject.m_transform.pos - pov.pos;
		const math::Vec2 spriteForwardVec = math::angle_to_vec(gameObject.m_transform.angle);
		const int animIDColumn = CalculateSpriteAnimationIDColumn(math::angle(povToSprite, spriteForwardVec));
		const int animID = walkAnimationID * SPRITESHEET_COLS + animIDColumn;
		
		re.RenderSprite(pov, gameObject.m_transform, spriteSheet, animID, 64);

		for (const Line& line: queue)
		{
			RenderEngine::TopDownLine tdl;
			tdl.line = line;
			tdl.color = Color{ 255, 255, 255, 255 };

			re.PushTopDownMapData(tdl);
		}
		queue.clear();
	}

	void RenderComponentCabron::OnLineDraw(const Line& line)
	{
		queue.push_back(line);
	}
}