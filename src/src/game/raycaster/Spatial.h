#pragma once

#include "RaycasterConstants.h"
#include "RCGFMath.h"
#include <vector>

namespace game::spatial
{
	bool line_of_sight(const math::Vec2& from, const math::Vec2& to);
	bool within_frustum(const math::Transform& pov, const math::Vec2& subject, float frustum = FOV);
	
	
	void init_path_finding(); // This must be called once before using do_pathfinding
	std::vector<math::Vec2> do_pathfinding(const math::Vec2& from, const math::Vec2& to);
}