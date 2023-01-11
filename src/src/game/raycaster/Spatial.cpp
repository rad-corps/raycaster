#include "Spatial.h"
#include "RaycasterConstants.h"
#include "Map.h" // FindWallHitPos
#include "EventSystem.h"
#include <algorithm>
#include <memory>
#include <cassert>

namespace
{
	struct PathfindingGridCell
	{
		PathfindingGridCell(int mapIndex)
			: mapIndex{ mapIndex }
			, adjacentCells{ game::map::get_adjacent_map_indices(mapIndex) }
			, parent{ nullptr }
		{
			assert(mapIndex >= 0);
			assert(mapIndex < game::MAP_SZ);
		}
		PathfindingGridCell()
			: mapIndex{ -1 }
			, adjacentCells{ -1, -1, -1, -1, -1, -1, -1, -1 }
		{}

		int Cost()
		{
			if (game::map::is_wall(mapIndex))
			{
				return 1000000;
			}
			else
			{
				return 1;
			}
		}

		math::Vec2 GetPos()
		{
			return game::map::to_world_position(mapIndex);
		}

		bool operator==(const PathfindingGridCell& pc)
		{
			return mapIndex == pc.mapIndex;
		}

		PathfindingGridCell* parent;

		int mapIndex;
		float g = 0; // cummulative edge cost
		float f = 0; // heuristic (distance from current to goal)
		std::array<int, 8> adjacentCells;
	};

	using PathfindingMap = std::array<PathfindingGridCell, game::MAP_SZ>;	
	PathfindingMap pathfindingMap;
	bool pathfindingInitialised = false;

	PathfindingGridCell* get_pathfinding_grid_cell(int index)
	{
		assert(index >= 0);
		assert(index < game::MAP_SZ);
		return &pathfindingMap[index];
	}
}

namespace game::spatial
{
	void init_path_finding()
	{
		for (int i = 0; i < MAP_SZ; ++i)
		{
			pathfindingMap[i] = PathfindingGridCell{ i };
		}
		pathfindingInitialised = true;
	}

	bool line_of_sight(const math::Vec2& from, const math::Vec2& to)
	{
		// get direction from subject to playerTransform.pos
		const math::Vec2 delta = to - from;
		const math::Vec2 dir = math::normalize(delta);
		float angle = math::vec_to_angle_pos(dir);

		// get distance to wall from subject position using direction
		const RayWallCollision collisionData = map::find_wall_hit_pos(from, angle);
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

	std::vector<math::Vec2> do_pathfinding(const math::Vec2& fromPos, const math::Vec2& destPos)
	{
		assert(pathfindingInitialised);

		// get the cell index for "from" and "to" positions
		const int fromIndex = map::to_map_index(fromPos);
		const int destIndex = map::to_map_index(destPos);

		PathfindingGridCell* origin = get_pathfinding_grid_cell(fromIndex);
		PathfindingGridCell* destination = get_pathfinding_grid_cell(destIndex);

		// create return value and open/closed lists
		std::vector<math::Vec2> ret;
		
		// non-owning pointers
		std::vector<PathfindingGridCell*> closedList;
		std::vector<PathfindingGridCell*> openList;


		PathfindingGridCell* currentGridCell = nullptr;
		
		origin->g = 0;
		origin->f = math::distance(fromPos, destPos);
		openList.push_back(origin);

		while (!openList.empty())
		{
			// lowest f score node to go to front (use lambda as we dont want to compare Node pointer addresses) 
			std::sort(openList.begin(), openList.end(), [](PathfindingGridCell* a, PathfindingGridCell* b) {return a->f < b->f; });

			currentGridCell = openList[0];

			// add it to the closed (traversed list) 
			closedList.push_back(openList[0]);

			// remove the node from the open list
			openList.erase(openList.begin());

			//if we have found the end node. 
			if (currentGridCell == destination)
			{
				assert(currentGridCell != nullptr);
				//success: bail from loop and create the return list
				break;
			}

			//loop through edges
			for (int edgeIndex : currentGridCell->adjacentCells)
			{
				if (edgeIndex == -1)
				{
					continue;
				}

				PathfindingGridCell* edgeGridCell = get_pathfinding_grid_cell( edgeIndex );

				// if edge node is not traversed (does not appear in closed list) 
				if (std::find(closedList.begin(), closedList.end(), edgeGridCell) == closedList.end())
				{
					// calculate a tentative f cost of the edge's end node
					float tentativeF = currentGridCell->g + edgeGridCell->Cost() + math::distance(edgeGridCell->GetPos(), destPos);

					//if the tentative f cost is less than the edge node's current f cost, update its data
					if (tentativeF < edgeGridCell->f)
					{
						//update scores since we have found a new lower f cost for this node
						edgeGridCell->parent = currentGridCell;
						edgeGridCell->g = currentGridCell->g + edgeGridCell->Cost();
						edgeGridCell->f = tentativeF;

						//add it to the open list if it is not there already
						if (std::find(openList.begin(), openList.end(), edgeGridCell) == openList.end())
						{
							openList.push_back(edgeGridCell);
						}
					}
				}
			}
		}

		//reconstruct the path	
		while (currentGridCell != nullptr)
		{
			ret.push_back(currentGridCell->GetPos());
			currentGridCell = currentGridCell->parent;
		}

		// erase last parent (starting point)
		// ret.erase(ret.end() - 1);

		// need to cleanup what we have done to the pathfinding data. 
		// todo: look for optimisation opportunity here. 
		init_path_finding();


		return ret;
	}
}