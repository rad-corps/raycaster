#pragma once

#include "gameobjects/base/AI.h"

namespace game
{
	class AI_WaypointFollow : public AI
	{
	public:
		AI_WaypointFollow() = delete;
		AI_WaypointFollow(std::vector<math::Vec2> waypointPositions)
			: m_waypointPositions{ waypointPositions }
		{}

		std::unique_ptr<AI> Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap) override;

	private:
		std::vector<math::Vec2> m_waypointPositions;
		int m_waypointIndex = 0;
	};

	class AI_Empty : public AI
	{
	public:
		std::unique_ptr<AI> Update(GameObject& subject, std::vector<GameObject>& gameObjects, const GameMap& gameMap) override;
	};
}
