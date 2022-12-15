#pragma once

#include "Globals.h"
#include "RCGFMath.h"
#include "SpriteSheet.h"
#include "RenderEngine.h"

namespace game
{
	class RenderingComponent
	{
	public:
		virtual ~RenderingComponent() = default;
		virtual void Render(const game::RenderEngine& re, const math::Transform& pov, const math::Transform& objTransform, rcgf::SpriteSheet* spriteSheet) = 0;
	};

	class CabronRenderer : public RenderingComponent
	{
	public:
		void Render(const game::RenderEngine& re, const math::Transform& pov, const math::Transform& objTransform, rcgf::SpriteSheet* spriteSheet) override;
	private:
		int CalculateSpriteAnimationID(float angle);
	};

	class BulletRenderer : public RenderingComponent
	{
	public:
		void Render(const game::RenderEngine& re, const math::Transform& pov, const math::Transform& objTransform, rcgf::SpriteSheet* spriteSheet) override;
	};
}
