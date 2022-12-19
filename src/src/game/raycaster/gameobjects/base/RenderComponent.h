#pragma once

#include "Globals.h"
#include "RCGFMath.h"
#include "SpriteSheet.h"
#include "RenderEngine.h"
#include "EventRecipient.h"

namespace game
{
	class GameObject;

	class RenderComponent : public EventRecipient
	{
	public:
		virtual ~RenderComponent() = default;
		virtual void Render(const game::RenderEngine& re, const math::Transform& pov, GameObject& gameObject, rcgf::SpriteSheet* spriteSheet) = 0;
	};
}
