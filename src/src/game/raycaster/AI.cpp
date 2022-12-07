#include "AI.h"
#include "Actor.h"

#include <iostream>

namespace game
{
	void AI_WaypointFollow::Update(Actor& sprite)
	{
		std::cout << "waypoint update: " << sprite.m_transform.angle << std::endl;
	}

	void AI_Empty::Update(Actor& sprite)
	{
		std::cout << "AI_Empty::update: " << sprite.m_transform.angle << std::endl;
	}
}