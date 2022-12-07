#pragma once

#include "RCGFMath.h"
#include <vector>
#include <memory>

namespace game
{
	struct Actor;

	class AI
	{
	public:
		virtual ~AI() {}
		virtual std::unique_ptr<AI> Update(Actor& actor) = 0;
	};

	class AI_WaypointFollow : public AI
	{
	public:
		AI_WaypointFollow(std::vector<math::Vec2> waypointPositions)
			: m_waypointPositions{ waypointPositions }
		{}
		~AI_WaypointFollow() {}

		std::unique_ptr<AI> Update(Actor& actor) override;
	
	private:
		std::vector<math::Vec2> m_waypointPositions;
		int m_waypointIndex = 0;
	};

	class AI_Empty : public AI
	{
	public:
		AI_Empty() {}
		~AI_Empty() {}
		std::unique_ptr<AI> Update(Actor& actor) override;
	};
}