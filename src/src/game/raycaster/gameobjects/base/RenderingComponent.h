#pragma once

#include "Globals.h"
#include "RCGFMath.h"
#include "SpriteSheet.h"
#include "RenderEngine.h"
#include "EventRecipient.h"

namespace game
{
	class RenderingComponent : public EventRecipient
	{
	public:
		virtual ~RenderingComponent() = default;
		virtual void Render(const game::RenderEngine& re, const math::Transform& pov, const math::Transform& objTransform, rcgf::SpriteSheet* spriteSheet) = 0;
	};
}
