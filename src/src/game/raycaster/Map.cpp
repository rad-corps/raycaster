#include "Map.h"
#include <cassert>
#include "RCGFMath.h"
#include <fstream>
#include <sstream>
#include "gameobjects/factory/GameObjectFactory.h"
#include "Player.h"


using math::PI;

namespace
{
	// todo: we need to be able to replace the map
	game::map::GameMap globalMap;
}

namespace game::map
{
	void init_global_map(const std::string& fileName)
	{
		// TODO: determine number of rows/columns and intialise globalMap with this data
		int cols = 0;
		int rows = 0;
		std::string line;
		std::fstream file{ fileName, std::ios::in };
		while (file >> line)
		{
			std::stringstream stream(line);
			std::string tmp;
			
			// make sure columns only counted once.
			if (rows == 0)
			{
				while (std::getline(stream, tmp, ','))
				{
					++cols;
				}
			}

			++rows;
		}
		globalMap = game::map::GameMap{ cols, rows };
	}

	void set_map(const std::string& filename, Player& player)
	{
		// clear gameobjects from factory??? 
		factory::Clear();

		std::cout << "set_map: " << filename << std::endl;

		// load file contents into string
		std::fstream wallsFile;
		std::fstream charactersFile;

		// Open an existing file
		std::string wallsFileName = filename + "_walls.csv";
		std::string charactersFileName = filename + "_characters.csv";

		init_global_map(wallsFileName);
		
		wallsFile.open(wallsFileName, std::ios::in);
		charactersFile.open(charactersFileName, std::ios::in);

		if (!wallsFile)
		{
			std::cout << "Failed to load file: " << wallsFileName << std::endl;
			return;
		}
		if (!charactersFile)
		{
			std::cout << "Failed to load map file: " << charactersFileName << std::endl;
			return;
		}

		
		int mapIndex = 0;
		// Read the Data from the file
		// as String Vector
		std::string wallCell, wallLine, characterCell, characterLine;

		// entire line goes into temp
		while (wallsFile >> wallLine) 
		{
			// used for breaking words
			std::stringstream wallLineStream(wallLine);

			// read every column data of a row and
			// store it in a string variable, 'word'
			while (std::getline(wallLineStream, wallCell, ','))
			{
				// set wall data to map
				const int mapCellType = stoi(wallCell);
				globalMap[mapIndex] = mapCellType;
				++mapIndex;
			}
		}

		mapIndex = 0;

		while (charactersFile >> characterLine)
		{
			std::stringstream characterLineStream(characterLine);

			while (std::getline(characterLineStream, characterCell, ','))
			{
				// set character data to map
				const int characterCellType = stoi(characterCell);

				//// set initial player pos/rotation
				if (characterCellType >= 0 && characterCellType < 8)
				{
					// calculate rotation
					int quarterRotations = characterCellType;
					float rotation = quarterRotations * (PI / 4.f);
					const math::Vec2 worldPos = to_world_position(mapIndex);

					// set the player position
					player.setTransform(math::Transform{ worldPos, rotation });
				}

				if (characterCellType >= 8 && characterCellType < 16)
				{
					// calculate rotation
					int quarterRotations = characterCellType - 8;
					float rotation = quarterRotations * (PI / 4.f);

					const math::Vec2 worldPos = to_world_position(mapIndex);

					// todo: convert mapCellType to angle
					factory::CreateCabron(math::Transform{ worldPos, rotation }, {});
				}
				++mapIndex;
			}
		}
	}

	const GameMap& get_map()
	{
		return globalMap;
	}
	bool facingDown(float angle_)
	{
		return 0 < angle_ && angle_ < PI;
	}
	bool facingUp(float angle_)
	{
		return PI < angle_;
	}
	bool facingRight(float angle_)
	{
		return angle_ < PI / 2 || 3 * PI / 2 < angle_;
	}
	bool facingLeft(float angle_)
	{
		return PI / 2 < angle_ && angle_ < 3 * PI / 2;
	}

	unsigned char getFacing(float angle_)
	{
		unsigned char ret = 0;
		if (facingUp(angle_)) ret |= Facing::UP;
		if (facingRight(angle_)) ret |= Facing::RIGHT;
		if (facingDown(angle_)) ret |= Facing::DOWN;
		if (facingLeft(angle_)) ret |= Facing::LEFT;
		return ret;
	}

	math::Vec2 to_world_position(int mapIndex)
	{
		int yIndex = mapIndex / globalMap.MAP_COLS();
		int xIndex = mapIndex % globalMap.MAP_COLS();
		return math::Vec2{ static_cast<float>(xIndex * MAP_CELL_PX + MAP_CELL_PX / 2), static_cast<float>(yIndex * MAP_CELL_PX + MAP_CELL_PX / 2) };
	}

	int to_map_index(int x, int y)
	{
		const int xIndex = x / MAP_CELL_PX;
		const int yIndex = y / MAP_CELL_PX;
		const int mapIndex = xIndex + yIndex * globalMap.MAP_COLS();

		// its possible to calculate a bad value with bad input, so lets
		// just agree to return -1
		if (mapIndex < 0 || globalMap.MAP_SZ() <= mapIndex)
		{
			//assert(false);
			return -1;
		}
		return mapIndex;
	}

	int to_map_index(const math::Vec2& pos)
	{
		return to_map_index(static_cast<int>(pos.x), static_cast<int>(pos.y));
	}

	std::array<int, 8> get_adjacent_map_indices(int idx)
	{
		/**
		*  [0][1][2]
		*  [3]idx[4]
		*  [5][6][7]
		*/

		//                       0   1   2   3   4   5   6   7
		std::array<int, 8> ret{ -1, -1, -1, -1, -1, -1, -1, -1 };

		// [0][1][2]
		if (idx >= globalMap.MAP_COLS()) // not the top row
		{
			if (idx % globalMap.MAP_COLS() != 0) // not the left most column
			{
				ret[0] = idx - globalMap.MAP_COLS() - 1;
			}
			
			ret[1] = idx - globalMap.MAP_COLS();

			if ((idx + 1) % globalMap.MAP_COLS() != 0) // not the right most column
			{
				ret[2] = idx - globalMap.MAP_COLS() + 1;
			}
		}

		// [3]idx[4]
		if (idx % globalMap.MAP_COLS() != 0) // not the left most column
		{
			ret[3] = idx - 1;
		}
		
		if ((idx + 1) % globalMap.MAP_COLS() != 0) // not the right most column
		{
			ret[4] = idx + 1;
		}

		// [5][6][7]
		if (idx + globalMap.MAP_COLS() < globalMap.MAP_SZ())
		{
			if (idx % globalMap.MAP_COLS() != 0) // not the left most column
			{
				ret[5] = idx + globalMap.MAP_COLS() - 1;
			}
			
			ret[6] = idx + globalMap.MAP_COLS();
			
			if ((idx + 1) % globalMap.MAP_COLS() != 0) // not the right most column
			{
				ret[7] = idx + globalMap.MAP_COLS() + 1;
			}
		}
		return ret;
	}

	int get_wall_spritesheet_index(int x, int y)
	{
		const int mapIndex = to_map_index(x, y);

		// hack (maybe return -1 is more suitable here? 0 is a valid spritesheet tile)
		if (mapIndex == -1)
		{
			return 0;
		}

		return globalMap[mapIndex];
	}

	// TODO: Remove?
	bool is_wall(float x, float y)
	{
		return is_wall(static_cast<int>(x), static_cast<int>(y));
	}

	bool is_wall(int x, int y)
	{
		const int mapIndex = to_map_index(x, y);

		// deal with -1 return value
		if (mapIndex == -1)
			return true;

		assert(mapIndex >= 0 && mapIndex < globalMap.MAP_SZ());
		return globalMap[mapIndex] >= 0;
	}

	bool is_wall(int index)
	{
		assert(index >= 0 && index < globalMap.MAP_SZ());
		return globalMap[index] >= 0;
	}



	bool is_in_wall(const SDL_Rect* playerVolume)
	{
		// player volume must be smaller than map cell size for this optimization to work
		assert(playerVolume->w <= MAP_CELL_PX && playerVolume->h <= MAP_CELL_PX);
		
		// check the 4 corners of the rect? 
		if (is_wall(playerVolume->x, playerVolume->y))                                     return true; // TL
		if (is_wall(playerVolume->x + playerVolume->w, playerVolume->y))                   return true; // TR
		if (is_wall(playerVolume->x + playerVolume->w, playerVolume->y + playerVolume->h)) return true; // BR
		if (is_wall(playerVolume->x, playerVolume->y + playerVolume->h))                   return true; // BL

		return false;
	}

	RayWallCollision find_wall_hit_pos(const math::Vec2& pos, float rayAngle)
	{
		RayWallCollision ret{};

		const float& x = pos.x;
		const float& y = pos.y;
		//const float& playerAngle = transform.angle;
		const unsigned char facing = getFacing(rayAngle);

		float rowIntersectDistance = 10000000.f;
		float colIntersectDistance = 10000000.f;

		// check horizontals
		// TODO: collapse LEFT and RIGHT branches together
		if (facing & Facing::RIGHT)
		{
			const int firstColIntersect = ((int)x / MAP_CELL_PX) * MAP_CELL_PX + MAP_CELL_PX;
			const float tempAngle = rayAngle;
			const float adjLen = firstColIntersect - x;
			const float oppLen = tan(tempAngle) * adjLen;
			const float xOffset = MAP_CELL_PX;
			const float yOffset = tan(tempAngle) * MAP_CELL_PX;
			float checkX = (float)firstColIntersect;
			float checkY = oppLen + y;

			while (!is_wall(checkX, checkY))
			{
				checkX += xOffset; checkY += yOffset;
			}

			colIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
			ret.distance = colIntersectDistance;
			ret.xHitPos = checkX;
			ret.yHitPos = checkY;

			ret.positionAlongWall = std::fmodf(ret.yHitPos, (float)MAP_CELL_PX);
			ret.positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
			ret.wallFaceDir = Facing::RIGHT;
		}
		else if (facing & Facing::LEFT)
		{
			const int firstColIntersect = ((int)x / MAP_CELL_PX) * MAP_CELL_PX;
			const float tempAngle = (2 * PI - rayAngle);
			const float adjLen = x - firstColIntersect;
			const float oppLen = tan(tempAngle) * adjLen;
			const float xOffset = -MAP_CELL_PX;
			const float yOffset = tan(tempAngle) * MAP_CELL_PX;
			float checkX = (float)firstColIntersect - 0.1f;
			float checkY = oppLen + y;

			while (!is_wall(checkX, checkY))
			{
				checkX += xOffset; checkY += yOffset;
			}
			colIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
			ret.distance = colIntersectDistance;
			ret.xHitPos = checkX;
			ret.yHitPos = checkY;

			ret.positionAlongWall = std::fmodf(ret.yHitPos, (float)MAP_CELL_PX);
			ret.positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
			ret.wallFaceDir = Facing::LEFT;
		}
		// check verticals
		// TODO: collapse UP and DOWN branches together
		if (facing & Facing::UP)
		{
			const int firstRowIntersect = ((int)y / MAP_CELL_PX) * MAP_CELL_PX;
			const float tempAngle = rayAngle - 3 * PI / 2;
			const float adjLen = y - firstRowIntersect;
			const float oppLen = tan(tempAngle) * adjLen;
			const float xOffset = tan(tempAngle) * MAP_CELL_PX;
			const float yOffset = -MAP_CELL_PX;
			float checkX = oppLen + x;
			float checkY = (float)firstRowIntersect - 0.1f;
			while (!is_wall(checkX, checkY))
			{
				checkX += xOffset; checkY += yOffset;
			}
			rowIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
			if (rowIntersectDistance < colIntersectDistance)
			{
				ret.distance = rowIntersectDistance;
				ret.xHitPos = checkX;
				ret.yHitPos = checkY;

				ret.positionAlongWall = std::fmodf(ret.xHitPos, (float)MAP_CELL_PX);
				ret.positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
				ret.wallFaceDir = Facing::UP;
			}
		}
		else if (facing & Facing::DOWN)
		{
			const int firstRowIntersect = ((int)y / MAP_CELL_PX) * MAP_CELL_PX + MAP_CELL_PX;
			const float tempAngle = PI / 2 - rayAngle;
			const float adjLen = firstRowIntersect - y;
			const float oppLen = tan(tempAngle) * adjLen;
			const float xOffset = tan(tempAngle) * MAP_CELL_PX;
			const float yOffset = MAP_CELL_PX;
			float checkX = oppLen + x;
			float checkY = (float)firstRowIntersect;
			while (!is_wall(checkX, checkY))
			{
				checkX += xOffset; checkY += yOffset;
			}
			rowIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
			if (rowIntersectDistance < colIntersectDistance)
			{
				ret.distance = rowIntersectDistance;
				ret.xHitPos = checkX;
				ret.yHitPos = checkY;

				ret.positionAlongWall = std::fmodf(ret.xHitPos, (float)MAP_CELL_PX);
				ret.positionAlongWall *= WALL_TEXTURE_SZ / MAP_CELL_PX;
				ret.wallFaceDir = Facing::DOWN;
			}
		}

		// TODO: move fisheye correction to the callsite for wall raycasting. 
		//       we do not want this correction when measuring distance from enemy to player
		// fisheye correction https://permadi.com/1996/05/ray-casting-tutorial-8/
		//const float angleDifference = rayAngle - playerAngle;
		//ret.distance *= cos(angleDifference);

		return ret;
	}
}