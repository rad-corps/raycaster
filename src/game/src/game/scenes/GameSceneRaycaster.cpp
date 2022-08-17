#include "GameSceneRaycaster.h"
#include "GameSceneMain.h"
#include "Globals.h"
#include <iostream>

using std::cout;
using std::endl;

namespace
{
	constexpr float PI = 3.14159265359f;
	constexpr int mapCols = 8;
	constexpr int mapRows = 8;
	constexpr int mapCellPx = 64;

	std::array<int, mapCols * mapRows> map =
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
			: x{ 100.f }, y{ 100.f }, angle{ 0.f }
		{}

		bool facingDown()
		{
			return 0 < angle && angle < PI;
		}
		bool facingUp()
		{
			return PI < angle;
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
		Player* p_player;

		RayTest() = delete;
		RayTest(Player* player_) :p_player{ player_ } {}

		void drawIntersect(int x, int y)
		{
			SDL_SetRenderDrawColor(global::instance.getRenderer(), 0xFF, 0xFF, 0, 0xFF);
			SDL_Rect r{ x-1, y-1, 3, 3 };
			SDL_RenderFillRect(global::instance.getRenderer(), &r);
		}

		void doRayTest()
		{

			Player& player = *p_player;

			// check horizontals

			// 1. are we facing up?? (up and down are inverted :( )
			//cout << "check horizontals" << endl;
			if (player.facingUp())
			{
				// find the nearest row line (y position)
				const int firstRowIntersect = ((int)player.y / mapCellPx) * mapCellPx;

				// draw a dot at the intersection
				drawIntersect((int) player.x, firstRowIntersect);

				// find the x position 
				const float tempAngle = player.angle - PI - (PI / 2);
				const float adjacentDist = player.y - firstRowIntersect;
				const float myTan = tan(tempAngle);
				const float oppositDist = myTan * adjacentDist;

				// draw a dot at the first intersect position
				drawIntersect((int)oppositDist + (int)player.x, firstRowIntersect);
			}

			// check verticals
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
		std::unique_ptr<RayTest> rt = std::make_unique<RayTest>(&player);
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
		m_impl->rt->doRayTest();
		
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
		case SDLK_e:
			m_impl->rt->doRayTest();
		}

	}

	void GameSceneRaycaster::keyUp(SDL_Keycode)
	{

	}
}
