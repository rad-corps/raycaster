#include "GameSceneRaycaster.h"
#include "GameSceneMain.h"
#include "Globals.h"
#include <iostream>
#include <bitset>

using std::cout;
using std::endl;

namespace
{
	// timing variables
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;


	constexpr float PI = 3.14159265359f;
	constexpr int mapCols = 8;
	constexpr int mapRows = 8;
	constexpr int mapSz = mapCols * mapRows;
	constexpr int mapCellPx = 64;
	constexpr float movementSpeed = 2.f;
	constexpr float rotationSpeed = 0.05f;
	constexpr float FOV_MIN = -PI / 5;
	constexpr float FOV_MAX = PI / 5;
	constexpr float ANGLE_STEP = ((PI / 5) * 2) / SCREEN_WIDTH;
	constexpr int RAYCAST_COLUMN_WIDTH = 1;

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
		1,0,0,0,1,0,0,1,
		1,0,0,0,1,0,0,1,
		1,0,1,0,1,0,0,1,
		1,0,1,0,1,0,0,1,
		1,1,1,0,1,1,0,1,
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
			if (dir == RotateDirection::Clockwise) angle += rotationSpeed;
			else angle -= rotationSpeed;
			if (angle < 0) angle += PI * 2;
			else if (2 * PI < angle) angle -= PI * 2;
		}

		void move(bool forward)
		{
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

		void doRayTest(float x, float y, float angle, unsigned char facing, bool showTopDown, bool show3D, int pxCol, int pxWidth)
		{
			//Player& player = *p_player;
			float rowIntersectDistance = 10000000.f;
			float colIntersectDistance = 10000000.f;
			float distance = 10000000.f;
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
				distance = colIntersectDistance;
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
				distance = colIntersectDistance;
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
					distance = rowIntersectDistance;
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
					distance = rowIntersectDistance;
					xIntersect = checkX;
					yIntersect = checkY;
				}
			}
			if (show3D)
			{
				SDL_Rect r;
				r.x = pxCol;
				r.w = pxWidth;
				r.h = (SCREEN_HEIGHT * 10) / ((int)distance+1);
				r.y = (SCREEN_HEIGHT - r.h) / 2;

				
				if (rowIntersectDistance < colIntersectDistance)
				{
					SDL_SetRenderDrawColor(global::instance.getRenderer(), 100, 100, 100, 0xFF);
				}
				else
				{
					SDL_SetRenderDrawColor(global::instance.getRenderer(), 50, 50, 50, 0xFF);
				}
				SDL_RenderFillRect(global::instance.getRenderer(), &r);
				
			}
			if (showTopDown)
			{
				SDL_SetRenderDrawColor(global::instance.getRenderer(), 0, 200, 0, 0xFF);
				SDL_RenderDrawLine(global::instance.getRenderer(), (int)x, (int)y, (int)xIntersect, (int)yIntersect);
			}
				
		}
	};
}

namespace game
{
	struct GameSceneRaycaster::Pimpl
	{
		Player player;
		RayTest rt;
		bool showTopDown = true;
		bool show3D = false;
		std::map<SDL_Keycode, bool> keyStates= {
			{SDLK_w, false},
			{SDLK_a, false},
			{SDLK_s, false},
			{SDLK_d, false},
			{SDLK_LEFT, false},
			{SDLK_RIGHT, false},
			{SDLK_UP, false},
			{SDLK_DOWN, false},
		};
		Pimpl()
		{}
	};

	GameSceneRaycaster::GameSceneRaycaster()
		: m_impl{ std::make_unique<Pimpl>() }
	{}

	void GameSceneRaycaster::update()
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

		if (m_impl->keyStates[SDLK_d] || m_impl->keyStates[SDLK_RIGHT]) m_impl->player.rotate(Player::RotateDirection::Clockwise);
		if (m_impl->keyStates[SDLK_a] || m_impl->keyStates[SDLK_LEFT])  m_impl->player.rotate(Player::RotateDirection::Anticlockwise);
		if (m_impl->keyStates[SDLK_w] || m_impl->keyStates[SDLK_UP])    m_impl->player.move(true);
		if (m_impl->keyStates[SDLK_s] || m_impl->keyStates[SDLK_DOWN])  m_impl->player.move(false);
	}

	void GameSceneRaycaster::render()
	{
		// raycast and render the 3d scene
		int pxCol = 0;
		for (float angle = FOV_MIN; angle < FOV_MAX; angle += ANGLE_STEP)
		{
			float finalAngle = m_impl->player.sumAngle(angle);
			m_impl->rt.doRayTest(m_impl->player.x, m_impl->player.y, finalAngle, getFacing(finalAngle), m_impl->showTopDown, m_impl->show3D, pxCol, RAYCAST_COLUMN_WIDTH);
			pxCol += RAYCAST_COLUMN_WIDTH;
		}

		// draw the map
		if (m_impl->showTopDown)
		{
			SDL_SetRenderDrawColor(global::instance.getRenderer(), 0, 0, 200, 0xFF);
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
		}

		// show x pixel coords for reference
		SDL_SetRenderDrawColor(global::instance.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		for (int i = 100; i < SCREEN_WIDTH; i += 100)
		{
			SDL_RenderDrawLine(global::instance.getRenderer(), i, SCREEN_HEIGHT, i, SCREEN_HEIGHT - 20);
		}
	}

	void GameSceneRaycaster::keyDown(SDL_Keycode keycode)
	{
		m_impl->keyStates[keycode] = true;

		switch (keycode)
		{
		case SDLK_a:
		case SDLK_d:
		case SDLK_w:
		case SDLK_s:
		case SDLK_LEFT:
		case SDLK_RIGHT:
		case SDLK_UP:
		case SDLK_DOWN:
			m_impl->keyStates[keycode] = true;
			break;
		}

		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
		case SDLK_ESCAPE:
			printf("switching to game state main\n");
			pushPendingState(std::make_unique<GameSceneMain>());
			break;
		case SDLK_TAB:
			m_impl->showTopDown = !m_impl->showTopDown;
			break;
		case SDLK_PERIOD:
			m_impl->show3D = !m_impl->show3D;
			break;
		}

	}

	void GameSceneRaycaster::keyUp(SDL_Keycode keycode)
	{
		m_impl->keyStates[keycode] = false;
	}
}
