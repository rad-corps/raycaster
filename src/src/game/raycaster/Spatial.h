#pragma once

#include "RaycasterConstants.h"
#include "RCGFMath.h"

namespace game
{
	bool line_of_sight(const math::Vec2& from, const math::Vec2& to);
	bool within_frustum(const math::Transform& pov, const math::Vec2& subject, float frustum = FOV);
}