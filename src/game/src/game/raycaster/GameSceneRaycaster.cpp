#include "GameSceneRaycaster.h"
#include "../scenes/GameSceneMain.h"
#include "Globals.h"
#include <iostream>
#include <bitset>
#include <chrono>
#include <sstream>
#include <iomanip> //std::setprecision
#include "Player.h"
#include "RaycastEngine.h"
#include "Map.h"

// typedef std::chrono::high_resolution_clock Clock;

using std::cout;
using std::endl;

namespace
{
	
	constexpr float FOV = PI / 3.f; // 60 degrees
	constexpr int X_PX_STEP = 1;
	constexpr int COLUMNS = SCREEN_WIDTH / X_PX_STEP;
	constexpr float START_ANGLE = 0.f;

	game::GameMap map =
	{
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,
		1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,
		1,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,
		1,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,
		1,0,0,0,0,1,0,1,1,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1,1,0,1,1,0,1,
		1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	struct SimplePerfCounter
	{
		void Start()
		{
			start = std::chrono::high_resolution_clock::now();
		}
		std::string Stop()
		{
			double result = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0;
			std::stringstream ss;
			ss << std::fixed << std::setprecision(1) << result;
			return ss.str();
		}
		std::chrono::steady_clock::time_point start;
	} perfCounter;
}

namespace game
{
	struct GameSceneRaycaster::Pimpl
	{
		Player player;
		bool showTopDown = false;
		bool show3D = true;
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

	}

	void GameSceneRaycaster::fixedUpdate()
	{
		if (m_impl->keyStates[SDLK_d] || m_impl->keyStates[SDLK_RIGHT]) m_impl->player.rotate(Player::RotateDirection::Clockwise);
		if (m_impl->keyStates[SDLK_a] || m_impl->keyStates[SDLK_LEFT])  m_impl->player.rotate(Player::RotateDirection::Anticlockwise);
		if (m_impl->keyStates[SDLK_w] || m_impl->keyStates[SDLK_UP])    m_impl->player.move(true, &map);
		if (m_impl->keyStates[SDLK_s] || m_impl->keyStates[SDLK_DOWN])  m_impl->player.move(false, &map);
	}

	void GameSceneRaycaster::render()
	{
		// render the 3D world from the player perspective
		{
			perfCounter.Start();
			ColumnRenderData columnRenderData[COLUMNS];
			constexpr float FOV_OFFSET = -(FOV / 2);
			for (int column = 0; column < COLUMNS; ++column)
			{
				// which x position pixel (column number in pixels)?
				const int xPx = column * X_PX_STEP;
				
				// px is what % across screen?
				const float pxPerc = xPx / (float)SCREEN_WIDTH;

				// calculate the angle to raycast based on the x screen position
				const float angle = pxPerc * FOV + FOV_OFFSET;

				// sum with the player's current angle to get the angle to send to the engine
				const float finalAngle = m_impl->player.sumAngle(angle);

				// cast the rays and render to screen
				columnRenderData[column] = raycast_engine::doRayTest(m_impl->player.x, m_impl->player.y, finalAngle, m_impl->player.angle, getFacing(finalAngle), xPx, X_PX_STEP, &map);
			}
			const std::string strRayTime = perfCounter.Stop();

			// draw the 3d scene
			perfCounter.Start();
			if (m_impl->show3D)
			{				
				for (int column = 0; column < COLUMNS; ++column)
				{
					ColumnRenderData& col = columnRenderData[column];
					SDL_SetRenderDrawColor(global::instance.getRenderer(), col.color.r, col.color.g, col.color.b, col.color.a);
					SDL_RenderFillRect(global::instance.getRenderer(), &col.rect);

					if (m_impl->showTopDown)
					{
						SDL_SetRenderDrawColor(global::instance.getRenderer(), 0, 200, 0, 0xFF);
						SDL_RenderDrawLine(global::instance.getRenderer(), col.ray.start.x, col.ray.start.y, col.ray.end.x, col.ray.end.y);
					}
				}
			}
			
			const std::string strRenderTime = perfCounter.Stop();

			global::instance.renderMonospaceText("Ray:" + strRayTime + " ms", SCREEN_WIDTH - 200, 0);
			global::instance.renderMonospaceText("Ren:" + strRenderTime + " ms", SCREEN_WIDTH - 200, 15);
		}

		// draw the map
		if (m_impl->showTopDown)
		{
			SDL_SetRenderDrawColor(global::instance.getRenderer(), 0, 0, 200, 0xFF);
			for (int row = 0; row < MAP_ROWS; ++row)
			{
				for (int col = 0; col < MAP_COLS; ++col)
				{
					// calc rect position and dimension
					SDL_Rect rect{ col * MAP_CELL_PX, row * MAP_CELL_PX, MAP_CELL_PX, MAP_CELL_PX };
					if (map[col + row * MAP_COLS] > 0)
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
