#pragma once

#include "RCGFMath.h"
#include <vector>
#include <memory>

namespace game
{
	class GameObject;

	class AI
	{
	public:
		virtual ~AI() {}
		virtual std::unique_ptr<AI> Update(GameObject& actor) = 0;
	};

	class AI_WaypointFollow : public AI
	{
	public:
		AI_WaypointFollow() = delete;
		AI_WaypointFollow(std::vector<math::Vec2> waypointPositions)
			: m_waypointPositions{ waypointPositions }
		{}

		std::unique_ptr<AI> Update(GameObject& actor) override;
	
	private:
		std::vector<math::Vec2> m_waypointPositions;
		int m_waypointIndex = 0;
	};

	class AI_Empty : public AI
	{
	public:
		std::unique_ptr<AI> Update(GameObject& actor) override;
	};

	class BulletBehavior : public AI
	{
	public:
		std::unique_ptr<AI> Update(GameObject& actor) override;
	};
}