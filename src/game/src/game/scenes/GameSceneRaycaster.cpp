#include "GameSceneRaycaster.h"
#include "GameSceneMain.h"
#include "Globals.h"
#include <iostream>
#include <bitset>

using std::cout;
using std::endl;

namespace
{
	constexpr float PI = 3.14159265359f;
	constexpr int mapCols = 8;
	constexpr int mapRows = 8;
	constexpr int mapSz = mapCols * mapRows;
	constexpr int mapCellPx = 64;

	enum Facing : unsigned char
	{
		UP = 0x0001,
		RIGHT = 0x0002,
		DOWN = 0x0004,
		LEFT = 0x0008,
	};

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


	std::array<int, mapSz> map =
	{
		1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,1,0,0,0,0,1,
		1,0,1,0,0,0,0,1,
		1,1,1,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1
	};

	struct Player
	{
		float x;
		float y;
		float angle;

		Player()
			: x{ 100.f }, y{ 400.f }, angle{ 0.f }
		{}

		// dont go more than one whole circle out of bounds
		float sumAngle(float add) const
		{
			add += angle;
			if (add < 0) add += PI * 2;
			else if (2 * PI < add) add -= PI * 2;
			return add;
		}

		unsigned char getFacing()
		{
			unsigned char ret = 0;
			if (facingUp()) ret |= Facing::UP;
			if (facingRight()) ret |= Facing::RIGHT;
			if (facingDown()) ret |= Facing::DOWN;
			if (facingLeft()) ret |= Facing::LEFT;
			return ret;
		}

		bool facingDown()
		{
			return 0 < angle && angle < PI;
		}
		bool facingUp()
		{
			return PI < angle;
		}
		bool facingRight()
		{
			return angle < PI / 2 || 3 * PI / 2 < angle;
		}
		bool facingLeft()
		{
			return PI / 2 < angle && angle < 3 * PI / 2;
		}
		void render()
		{
			// draw the player
			int constexpr playerDiameter = 2;
			SDL_SetRenderDrawColor(global::instance.getRenderer(), 100, 200, 0, 0xFF);
			SDL_Rect r{ (int)x - playerDiameter,(int)y - playerDiameter,playerDiameter*2,playerDiameter*2 };
			SDL_RenderFillRect(global::instance.getRenderer(), &r);

			// draw line to show rotation
			//  - calculate sin and cos of angle
			float sinTheta = sin(angle) * 20;
			float cosTheta = cos(angle) * 20;

			SDL_RenderDrawLine(global::instance.getRenderer(), (int)x, (int)y, (int)cosTheta + (int)x, (int)sinTheta + (int)y);
		}

		enum class RotateDirection
		{
			Clockwise,
			Anticlockwise
		};
		
		void rotate(RotateDirection dir)
		{
			constexpr float rotationSpeed = 0.1f;
			if (dir == RotateDirection::Clockwise) angle += rotationSpeed;
			else angle -= rotationSpeed;
			if (angle < 0) angle += PI * 2;
			else if (2 * PI < angle) angle -= PI * 2;
		}

		void move(bool forward)
		{
			constexpr int movementSpeed = 5;
			float movementAngle = angle;
			if (!forward)
			{
				PI < movementAngle ? movementAngle -= PI : movementAngle += PI;
			}
			y += sin(movementAngle) * movementSpeed;
			x += cos(movementAngle) * movementSpeed;
		}
	};

	struct RayTest
	{
		//Player* p_player;

		//RayTest() = delete;
		//RayTest(Player* player_) :p_player{ player_ } {}

		void drawIntersect(int x, int y)
		{
			SDL_SetRenderDrawColor(global::instance.getRenderer(), 0xFF, 0xFF, 0, 0xFF);
			SDL_Rect r{ x-1, y-1, 3, 3 };
			SDL_RenderFillRect(global::instance.getRenderer(), &r);
		}

		int toMapIndex(float x, float y)
		{
			const int xIndex = ((int)x / mapCellPx);
			const int yIndex = ((int)y / mapCellPx);
			const int mapIndex = xIndex + yIndex * mapCols;
			
			// its possible to calculate a bad value with bad input, so lets
			// just agree to return -1
			if (mapIndex < 0 || mapSz <= mapIndex )
			{
				return -1;
			}
			return mapIndex;
		}

		bool isWall(float x, float y)
		{
			const int mapIndex = toMapIndex(x, y);

			// deal with -1 return value
			if (mapIndex == -1)
				return true;

			assert(0 <= mapIndex && mapIndex < mapSz);
			return 0 < map[mapIndex];
		}

		void doRayTest(float x, float y, float angle, unsigned char facing)
		{
			//Player& player = *p_player;
			float rowIntersectDistance = 10000000.f;
			float colIntersectDistance = 10000000.f;
			float xIntersect = 10000000.f;
			float yIntersect = 10000000.f;

			// check horizontals
			if (facing & Facing::RIGHT)
			{
				//cout << "right" << endl;
				const int firstColIntersect = ((int)x / mapCellPx) * mapCellPx + mapCellPx;
				const float tempAngle = angle;
				const float adjLen = firstColIntersect - x;
				const float oppLen = tan(tempAngle) * adjLen;
				const float xOffset = mapCellPx;
				const float yOffset = tan(tempAngle) * mapCellPx;
				float checkX = (float)firstColIntersect;
				float checkY = oppLen + y;

				while (!isWall(checkX, checkY))
				{
					checkX += xOffset; checkY += yOffset;
				}
				colIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
				xIntersect = checkX;
				yIntersect = checkY;
			}
			else if (facing & Facing::LEFT) 
			{
				//cout << "left" << endl;
				const int firstColIntersect = ((int)x / mapCellPx) * mapCellPx;
				const float tempAngle = (2 * PI - angle);
				const float adjLen = x - firstColIntersect;
				const float oppLen = tan(tempAngle) * adjLen;
				const float xOffset = -mapCellPx;
				const float yOffset = tan(tempAngle) * mapCellPx;
				float checkX = (float)firstColIntersect - 0.1f;
				float checkY = oppLen + y;

				while (!isWall(checkX, checkY))
				{
					checkX += xOffset; checkY += yOffset;
				}
				colIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
				xIntersect = checkX;
				yIntersect = checkY;
			}

			// check verticals
			if (facing & Facing::UP)
			{
				//cout << "up" << endl;
				const int firstRowIntersect = ((int)y / mapCellPx) * mapCellPx;
				const float tempAngle = angle - 3 * PI / 2;
				const float adjLen = y - firstRowIntersect;
				const float oppLen = tan(tempAngle) * adjLen;
				const float xOffset = tan(tempAngle) * mapCellPx;
				const float yOffset = -mapCellPx;
				float checkX = oppLen + x;
				float checkY = (float)firstRowIntersect - 0.1f;
				while (!isWall(checkX, checkY))
				{
					checkX += xOffset; checkY += yOffset;
				}			
				rowIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
				if (rowIntersectDistance < colIntersectDistance)
				{
					xIntersect = checkX;
					yIntersect = checkY;
				}
			}
			else if (facing & Facing::DOWN)
			{
				//cout << "down" << endl;
				const int firstRowIntersect = ((int)y / mapCellPx) * mapCellPx + mapCellPx;
				const float tempAngle = PI / 2 - angle;
				const float adjLen = firstRowIntersect - y;
				const float oppLen = tan(tempAngle) * adjLen;
				const float xOffset = tan(tempAngle) * mapCellPx;
				const float yOffset = mapCellPx;
				float checkX = oppLen + x;
				float checkY = (float)firstRowIntersect;
				while (!isWall(checkX, checkY))
				{
					checkX += xOffset; checkY += yOffset;
				}
				rowIntersectDistance = sqrt((float)pow(checkX - x, 2) + (float)pow(checkY - y, 2));
				if (rowIntersectDistance < colIntersectDistance)
				{
					xIntersect = checkX;
					yIntersect = checkY;
				}
			}
			SDL_RenderDrawLine(global::instance.getRenderer(), (int)x, (int)y, (int)xIntersect, (int)yIntersect);
		}

		void render()
		{

		}

	};
}

namespace game
{
	struct GameSceneRaycaster::Pimpl
	{
		Player player;
		RayTest rt;
		Pimpl()
		{}
	};

	GameSceneRaycaster::GameSceneRaycaster()
		: m_impl{ std::make_unique<Pimpl>() }
	{}

	void GameSceneRaycaster::update()
	{
	}

	void GameSceneRaycaster::render()
	{
		// draw a grid to match the map
		SDL_SetRenderDrawColor(global::instance.getRenderer(), 0, 0, 200, 0xFF);

		// draw the map
		for (int row = 0; row < mapRows; ++row)
		{
			for (int col = 0; col < mapCols; ++col)
			{
				// calc rect position and dimension
				SDL_Rect rect{ col * mapCellPx, row * mapCellPx, mapCellPx, mapCellPx };
				if (map[col + row * mapCols] > 0)
				{
					SDL_RenderFillRect(global::instance.getRenderer(), &rect);
				}
				else
				{
					SDL_RenderDrawRect(global::instance.getRenderer(), &rect);
				}
			}
		}

		m_impl->player.render();

		for (float angle = -PI / 5; angle < PI / 5; angle += PI / 150)
		{
			float finalAngle = m_impl->player.sumAngle(angle);
			m_impl->rt.doRayTest(m_impl->player.x, m_impl->player.y, finalAngle, getFacing(finalAngle));
		}
			
	}

	void GameSceneRaycaster::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
		case SDLK_ESCAPE:
			printf("switching to game state main\n");
			pushPendingState(std::make_unique<GameSceneMain>());
			break;

		case SDLK_a:
			m_impl->player.rotate(Player::RotateDirection::Anticlockwise);
			break;
		case SDLK_d:
			m_impl->player.rotate(Player::RotateDirection::Clockwise);
			break;
		case SDLK_w:
			m_impl->player.move(true);
			break;
		case SDLK_s:
			m_impl->player.move(false);
			break;
		}

	}

	void GameSceneRaycaster::keyUp(SDL_Keycode)
	{

	}
}
