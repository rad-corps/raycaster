#pragma once

#include "RCGFMath.h"
#include <vector>

namespace game
{
	struct Actor;

	class AI
	{
	public:
		virtual ~AI() {}
		virtual void Update(Actor& actor) = 0;
	};

	class AI_WaypointFollow : public AI
	{
	public:
		AI_WaypointFollow(std::vector<math::Vec2> waypointPositions)
			: waypointPositions{ waypointPositions }
		{}
		~AI_WaypointFollow() {}

		void Update(Actor& actor) override;
	
	private:
		std::vector<math::Vec2> waypointPositions;
	};

	class AI_Empty : public AI
	{
	public:
		AI_Empty() {}
		~AI_Empty() {}
		void Update(Actor& actor) override;
	};
}