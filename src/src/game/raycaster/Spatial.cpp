#include "Spatial.h"
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

	bool within_frustum(const math::Transform& pov, const math::Vec2& subject, float frustum)
	{
		// check frustum 
		const math::Vec2 povToSubject = subject - pov.pos;

		const float playerAngleFromEnemy = math::vec_to_angle_pos(povToSubject);
		float directionPlayerDifference = math::sum_angle(-playerAngleFromEnemy, pov.angle);
		if (directionPlayerDifference > math::PI)
			directionPlayerDifference -= 2 * math::PI;		

		Color color = { 0xFF, 0xFF, 0x0, 0xFF }; // yellow
		bool ret = false;
		if (abs(directionPlayerDifference) < frustum / 2.f)
		{
			color = { 0xFF, 0x0, 0x0, 0xFF }; // red
			ret = true;
		}

		// draw the frustum
		{

			float leftAngle = math::sum_angle(pov.angle, -frustum / 2.f);
			float rightAngle = math::sum_angle(pov.angle, frustum / 2.f);
			math::Vec2 leftFrustumVec = math::scale(math::angle_to_vec(leftAngle), 200.f);
			math::Vec2 rightFrustumVec = math::scale(math::angle_to_vec(rightAngle), 200.f);

			ColouredLine l;
			l.color = color;
			l.line.start.x = pov.pos.x;
			l.line.start.y = pov.pos.y;
			l.line.end.x = pov.pos.x + leftFrustumVec.x;
			l.line.end.y = pov.pos.y + leftFrustumVec.y;
			events::publish(events::ColouredLineEvent{ l });

			l.line.end.x = pov.pos.x + rightFrustumVec.x;
			l.line.end.y = pov.pos.y + rightFrustumVec.y;
			events::publish(events::ColouredLineEvent{ l });
		}


		return ret;
	}
}