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

// typedef std::chrono::high_resolution_clock Clock;

using std::cout;
using std::endl;

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



	
}

namespace game
{
	struct GameSceneRaycaster::Pimpl
	{
		Player player;
		bool showTopDown = false;
		rcgf::Texture wallTexture{ "./img/wall_64.png" };
		
		// diagnostic output
		std::string renderTime;
		std::string fps;
		
		ColumnRenderData columnRenderDataArray[COLUMNS] = {};
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
		Pimpl()
		{
			wallTexture.printDebugInfo();
		}
	};


	GameSceneRaycaster::GameSceneRaycaster()
		: m_impl{ std::make_unique<Pimpl>() }
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

	void GameSceneRaycaster::render()
	{
		// render the 3D world from the player perspective
		{
#ifdef RENDER_DEBUG_VALUES
			perfCounter.Start();
#endif

			constexpr float FOV_OFFSET = -(FOV / 2);

			int screenColumnNumber = 0;
			for (ColumnRenderData& crd : m_impl->columnRenderDataArray)
			{
				// which x position pixel (column number in pixels)?
				screenColumnNumber += X_PX_STEP;

				// px is what % across screen?
				const float pxPerc = screenColumnNumber / (float)SCREEN_WIDTH;

				// calculate the angle to raycast based on the x screen position
				const float angle = pxPerc * FOV + FOV_OFFSET;

				// sum with the player's current angle to get the angle to send to the engine
				const float finalAngle = m_impl->player.sumAngle(angle);

				// cast the ray to calculate wall height for this column
				crd = raycast_engine::doRayTest(m_impl->player.transform, finalAngle, screenColumnNumber, &map);

				// draw the 3d scene
				// draw walls
				const SDL_Rect textureClip{ crd.textureXPos,0,1,WALL_TEXTURE_SZ };
				m_impl->wallTexture.render2(&textureClip, &crd.rect);

				// draw floors
				// 1. start from the bottom of the wall
				for (int y = crd.rect.y + crd.rect.h; y < SCREEN_HEIGHT; ++y)
				{
					//// playerHeight / 
					//const float ratio = 

					const int px = y - CENTER_Y;
					const int DIST_PROJECTION_PLANEb = static_cast<int>((SCREEN_WIDTH / 2) / tan(FOV / 2));
					//const float straightDistancePlayerToFloorIntersect = (float)CENTER_Y / (float)px * DIST_PROJECTION_PLANEb;
					const int playerHeight = MAP_CELL_PX / 2;
					const float ratio = (float)playerHeight / (float)px;
					const float angleDifference = finalAngle - m_impl->player.transform.angle;
					const float diagonalDistance = DIST_PROJECTION_PLANEb * ratio * cos(angleDifference);
					float xEnd = diagonalDistance * cos(finalAngle);
					float yEnd = diagonalDistance * sin(finalAngle);
					xEnd += m_impl->player.transform.x;
					yEnd += m_impl->player.transform.y;

					// get the texture coordinate from worldPos
					float txCoordX = std::fmodf(xEnd, (float)MAP_CELL_PX);
					txCoordX *= WALL_TEXTURE_SZ / MAP_CELL_PX;
					float txCoordY = std::fmodf(yEnd, (float)MAP_CELL_PX);
					txCoordY *= WALL_TEXTURE_SZ / MAP_CELL_PX;

					const Color color = m_impl->wallTexture.getPixelColor((int)txCoordX, (int)txCoordY);
					SDL_Rect rect;
					rect.x = screenColumnNumber;
					rect.y = y;
					rect.w = X_PX_STEP;
					rect.h = X_PX_STEP;
					SDL_SetRenderDrawColor(global::instance.getRenderer(), color.r, color.g, color.b, color.a);
					SDL_RenderFillRect(global::instance.getRenderer(), &rect);
				}
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
				global::instance.renderMonospaceText("ray:" + strRayTime + " ms", textXPos, yOffset);
				global::instance.renderMonospaceText("drw:" + strRenderTime + " ms", textXPos, yOffset += 15);
				global::instance.renderMonospaceText("ren:" + m_impl->renderTime + " ms", textXPos, yOffset += 15);
				global::instance.renderMonospaceText("fps:" + m_impl->fps, textXPos, yOffset += 15);
				
			}
#endif
		}

		// draw the map
		if (m_impl->showTopDown)
		{
			SDL_SetRenderDrawColor(global::instance.getRenderer(), 150, 100, 0, 0xFF);
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

			for (const ColumnRenderData& col : m_impl->columnRenderDataArray)
			{
				if (m_impl->showTopDown)
				{
					SDL_RenderDrawLine(global::instance.getRenderer(), col.ray.start.x, col.ray.start.y, col.ray.end.x, col.ray.end.y);
				}
			}
		}
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
			pushPendingState(std::make_unique<GameSceneMain>());
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
