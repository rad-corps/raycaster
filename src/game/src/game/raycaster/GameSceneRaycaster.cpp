#include "GameSceneRaycaster.h"
#include "../scenes/GameSceneMain.h"
#include "Globals.h"
#include <iostream>
#include <bitset>

#include <sstream>
#include <iomanip> //std::setprecision
#include "Player.h"
#include "RaycastEngine.h"
#include "Map.h"
#include "Texture.h"
#include "RaycasterConstants.h"
#include <vector>

#define RENDER_DEBUG_VALUES

namespace
{
	SimplePerfCounter perfCounter;

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


	const SDL_Point enemyPosition{ 58,64};
	
}

namespace game
{
	struct GameSceneRaycaster::Pimpl
	{
		Player player;
		bool showTopDown = false;
		rcgf::Texture wallTexture;
		rcgf::Texture enemyTexture;
		
		// diagnostic output
		std::string renderTime;
		std::string fps;
		
		std::map<SDL_Keycode, bool> keyStates= {
			{SDLK_w, false},
			{SDLK_a, false},
			{SDLK_s, false},
			{SDLK_d, false},
			{SDLK_LEFT, false},
			{SDLK_RIGHT, false},
			{SDLK_UP, false},
			{SDLK_DOWN, false},
			{SDLK_LCTRL, false},
		};
		Pimpl(SDL_Renderer* renderer)
			: wallTexture{ renderer, "./img/wall_64.png" },
			enemyTexture{ renderer, "./img/enemy_01.png" }
		{
			wallTexture.printDebugInfo();
		}
		Pimpl() = delete;
	};


	GameSceneRaycaster::GameSceneRaycaster(SDL_Renderer* renderer, TTF_Font* font)
		: m_impl{ std::make_unique<Pimpl>(renderer) }
		, m_renderer{renderer}
		, m_font{font}
	{}

	void GameSceneRaycaster::update()
	{

	}

	void GameSceneRaycaster::sendData(const std::string& renderTime, const std::string& fps)
	{
		m_impl->renderTime = renderTime;
		m_impl->fps = fps;
	}

	void GameSceneRaycaster::fixedUpdate()
	{
		if (!m_impl->keyStates[SDLK_LCTRL])
		{
			// rotate
			if (m_impl->keyStates[SDLK_d] || m_impl->keyStates[SDLK_RIGHT]) m_impl->player.rotate(Player::RotateDirection::Clockwise);
			if (m_impl->keyStates[SDLK_a] || m_impl->keyStates[SDLK_LEFT])  m_impl->player.rotate(Player::RotateDirection::Anticlockwise);
		}
		else
		{
			// strafe
			if (m_impl->keyStates[SDLK_d] || m_impl->keyStates[SDLK_RIGHT]) m_impl->player.move(PI / 2.f, &map);
			if (m_impl->keyStates[SDLK_a] || m_impl->keyStates[SDLK_LEFT])  m_impl->player.move(PI + PI * 0.5f, &map);
		}
		
		// forward and backward movement
		if (m_impl->keyStates[SDLK_w] || m_impl->keyStates[SDLK_UP])    m_impl->player.move(0.f, &map);
		if (m_impl->keyStates[SDLK_s] || m_impl->keyStates[SDLK_DOWN])  m_impl->player.move(PI, &map);
	}

	void GameSceneRaycaster::render(SDL_Renderer* renderer)
	{
		// 4100
		renderer;
		// render the 3D world from the player perspective
		{
#ifdef RENDER_DEBUG_VALUES
			perfCounter.Start();
#endif

			constexpr float FOV_OFFSET = -(FOV / 2);

			int screenColumnNumber = 0;

			for (int column = 0; column < COLUMNS; ++column)
			{
				// which x position pixel (column number in pixels)?
				screenColumnNumber += X_PX_STEP;

				// px is what % across screen?
				const float pxPerc = screenColumnNumber / (float)SCREEN_WIDTH;

				// calculate the angle to raycast based on the x screen position
				const float angle = pxPerc * FOV + FOV_OFFSET;

				// sum with the player's current angle to get the angle to send to the engine
				const float rayAngle = m_impl->player.sumAngle(angle);

				// cast the ray to calculate wall height for this column
				ColumnRenderData crd = raycast_engine::doRayTest(m_impl->player.transform, rayAngle, screenColumnNumber, &map);

				// hack, draw the ray line here since we stopped caching the ColumnRenderData				
				if (m_impl->showTopDown)
				{
					SDL_SetRenderDrawColor(m_renderer, 200, 0, 200, 0xFF);
					SDL_RenderDrawLine(m_renderer, crd.ray.start.x, crd.ray.start.y, crd.ray.end.x, crd.ray.end.y);
				}

				// draw walls
				const SDL_Rect textureClip{ crd.textureXPos,0,1,WALL_TEXTURE_SZ };
				m_impl->wallTexture.render2(&textureClip, &crd.rect);

#ifdef RENDER_FLOORS
				raycast_engine::drawFloorColumn(m_impl->player.transform, crd, screenColumnNumber, rayAngle, &m_impl->wallTexture);
#endif
			}

#ifdef RENDER_DEBUG_VALUES
			const std::string strRayTime = perfCounter.Stop();
			perfCounter.Start();
#endif


			

#ifdef RENDER_DEBUG_VALUES
			{
				int yOffset = 0;
				const std::string strRenderTime = perfCounter.Stop();
				constexpr int textXPos = SCREEN_WIDTH - 120;
				global::Global::renderMonospaceText("ray:" + strRayTime + " ms", textXPos, yOffset);
				global::Global::renderMonospaceText("drw:" + strRenderTime + " ms", textXPos, yOffset += 15);
				global::Global::renderMonospaceText("ren:" + m_impl->renderTime + " ms", textXPos, yOffset += 15);
				global::Global::renderMonospaceText("fps:" + m_impl->fps, textXPos, yOffset += 15);
				
			}
#endif
		}

		// draw the map
		if (m_impl->showTopDown)
		{
			SDL_SetRenderDrawColor(m_renderer, 150, 100, 0, 0xFF);
			for (int row = 0; row < MAP_ROWS; ++row)
			{
				for (int col = 0; col < MAP_COLS; ++col)
				{
					// calc rect position and dimension
					SDL_Rect rect{ col * MAP_CELL_PX, row * MAP_CELL_PX, MAP_CELL_PX, MAP_CELL_PX };
					if (map[col + row * MAP_COLS] > 0)
					{
						SDL_RenderFillRect(m_renderer, &rect);
					}
					else
					{
						SDL_RenderDrawRect(m_renderer, &rect);
					}
				}
			}

			m_impl->player.render(m_renderer);

			SDL_Rect enemyRect{
				enemyPosition.x - 2,
				enemyPosition.y - 2,
				4, 4
			};
			SDL_SetRenderDrawColor(m_renderer, 255, 0, 100, 0xFF);
			SDL_RenderFillRect(m_renderer, &enemyRect);

			// TODO: now that we have removed columnRenderDataArray from the heap:
			// fix displaying the player rays
			//for (const ColumnRenderData& col : m_impl->columnRenderDataArray)
			//{
			//	if (m_impl->showTopDown)
			//	{
			//		SDL_RenderDrawLine(global::instance.getRenderer(), col.ray.start.x, col.ray.start.y, col.ray.end.x, col.ray.end.y);
			//	}
			//}
		}
		m_impl->enemyTexture.render(SCREEN_WIDTH - 64, SCREEN_HEIGHT - 64);
	}
	
	void GameSceneRaycaster::mouseDown(int button, int x, int y)
	{
		std::cout << "mouseDown: " << button << " " << x << " " << y << std::endl;
		if (y > CENTER_Y)
		{
			// formula rowDistance = cameraHeight (half the screen height) / (yPx - cameraHeight)
			const int rowDistance = CENTER_Y / (y - CENTER_Y);
			std::cout << rowDistance << std::endl;
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
			pushPendingState(std::make_unique<GameSceneMain>(m_renderer, m_font));
			break;
		case SDLK_TAB:
			m_impl->showTopDown = !m_impl->showTopDown;
			break;
		}

	}

	void GameSceneRaycaster::keyUp(SDL_Keycode keycode)
	{
		m_impl->keyStates[keycode] = false;
	}
}
