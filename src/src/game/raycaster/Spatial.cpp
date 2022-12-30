#include "Spatial.h"
#include "RaycasterConstants.h"
#include "Map.h" // FindWallHitPos
#include "EventSystem.h"

namespace game
{
	bool line_of_sight(const math::Vec2& from, const math::Vec2& to)
	{
		// get direction from subject to playerTransform.pos
		const math::Vec2 delta = to - from;
		const math::Vec2 dir = math::normalize(delta);
		float angle = math::vec_to_angle_pos(dir);

		// get distance to wall from subject position using direction
		const RayWallCollision collisionData = FindWallHitPos(from, angle, &getMap());
		const float distance = math::magnitude(delta);
		bool ret = false;

		Color color = { 0xFF,0xA5,0x00,0xFF }; // orange

		if (collisionData.distance > distance)
		{
			color = Color{ 0xFF,0x00,0x00,0xFF }; // red
			ret = true;
		}

		// line from enemy to collisionData sent to RenderEngine to draw top down map
		ColouredLine l;
		l.line.start.x = from.x;
		l.line.start.y = from.y;
		l.line.end.x = collisionData.xHitPos;
		l.line.end.y = collisionData.yHitPos;
		l.color = color;
		events::publish(events::ColouredLineEvent{ l });

		return ret;
	}
}