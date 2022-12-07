#include "AI.h"
#include "Actor.h"

#include <iostream>

namespace game
{
#pragma warning( push )
#pragma warning( disable : 4100)

	std::unique_ptr<AI> AI_WaypointFollow::Update(Actor& sprite)
	{
		//std::cout << "waypoint update: " << sprite.m_transform.angle << std::endl;
		return nullptr;
	}
#pragma warning( pop ) 


	std::unique_ptr<AI> AI_Empty::Update(Actor& sprite)
	{
		std::cout << "AI_Empty::update: " << sprite.m_transform.angle << std::endl;
		return nullptr;
	}
}