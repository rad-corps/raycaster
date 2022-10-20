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
		bool show3D = true;
		rcgf::Texture wallTexture{ "./img/wall_64.png" };
		Color testColor;
		
		// diagnostic output
		std::string renderTime;
		std::string fps;
		
		ColumnRenderData columnRenderDataArray[COLUMNS];
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
		void PopulateVertPixelData(ColumnRenderData& crd, const int wallXPos)
		{
			const int wallHeight = crd.rect.h;
			

			// divide the wall height by the texture size to find how high to make each pixel from the texture
			const float renderedPxHeight = wallHeight / (float)WALL_TEXTURE_SZ;

			// work top to bottom, and keep track of the last pixel
			
			for (int yTexturePx = 0; yTexturePx < WALL_TEXTURE_SZ; ++yTexturePx)
			{
				crd.verticalPixelArray[yTexturePx].color = wallTexture.getPixelColor(wallXPos, yTexturePx);
				const int yScreenPos = (int)(crd.rect.y + yTexturePx * renderedPxHeight);
				crd.verticalPixelArray[yTexturePx].rect.y = yScreenPos;
				crd.verticalPixelArray[yTexturePx].rect.h = (int)renderedPxHeight + 1;
				crd.verticalPixelArray[yTexturePx].rect.x = crd.rect.x;
				crd.verticalPixelArray[yTexturePx].rect.w = crd.rect.w;
			}
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
			if (m_impl->keyStates[SDLK_d] || m_impl->keyStates[SDLK_RIGHT]) m_impl->player.rotate(Player::RotateDirection::Clockwise);
			if (m_impl->keyStates[SDLK_a] || m_impl->keyStates[SDLK_LEFT])  m_impl->player.rotate(Player::RotateDirection::Anticlockwise);
		}
		else
		{
			if (m_impl->keyStates[SDLK_d] || m_impl->keyStates[SDLK_RIGHT]) m_impl->player.strafe(true, &map);
			if (m_impl->keyStates[SDLK_a] || m_impl->keyStates[SDLK_LEFT])  m_impl->player.strafe(false, &map);
		}
		
		if (m_impl->keyStates[SDLK_w] || m_impl->keyStates[SDLK_UP])    m_impl->player.move(true, &map);
		if (m_impl->keyStates[SDLK_s] || m_impl->keyStates[SDLK_DOWN])  m_impl->player.move(false, &map);
	}

	void GameSceneRaycaster::render()
	{
		// render the 3D world from the player perspective
		{
#ifdef RENDER_DEBUG_VALUES
			perfCounter.Start();
#endif
			
			constexpr float FOV_OFFSET = -(FOV / 2);

			int xPx = 0;
			for (ColumnRenderData& crd : m_impl->columnRenderDataArray)
			{
				// which x position pixel (column number in pixels)?
				xPx += X_PX_STEP;

				// px is what % across screen?
				const float pxPerc = xPx / (float)SCREEN_WIDTH;

				// calculate the angle to raycast based on the x screen position
				const float angle = pxPerc * FOV + FOV_OFFSET;

				// sum with the player's current angle to get the angle to send to the engine
				const float finalAngle = m_impl->player.sumAngle(angle);

				// cast the rays and render to screen
				crd = raycast_engine::doRayTest(m_impl->player.transform, finalAngle, xPx, &map);

				m_impl->PopulateVertPixelData(crd, crd.textureXPos);
			}

#ifdef RENDER_DEBUG_VALUES
			const std::string strRayTime = perfCounter.Stop();
			perfCounter.Start();
#endif
			int numPixelsDrawn = 0;
			// draw the 3d scene
			if (m_impl->show3D)
			{
				
				for (const ColumnRenderData& col : m_impl->columnRenderDataArray)
				{
					 // PAINT EVERY TEXTURE PIXEL
					 //////////////////////////////
					//int prevY = -1;
					//for (const RenderableTexturePixel& rtp : col.verticalPixelArray)
					//{
					//	// don't bother drawing off screen
					//	if (rtp.rect.y >= 0 && rtp.rect.y <= SCREEN_HEIGHT)
					//	{
					//		// no point overwriting previous data in the column
					//		if (rtp.rect.y > prevY)
					//		{
					//			SDL_SetRenderDrawColor(global::instance.getRenderer(), rtp.color.r, rtp.color.g, rtp.color.b, rtp.color.a);
					//			SDL_RenderFillRect(global::instance.getRenderer(), &rtp.rect);
					//			prevY = rtp.rect.y;
					//			++numPixelsDrawn;
					//		}
					//	}
					//}


					// PAINT ONLY COLUMNS
					const SDL_Rect textureClip{ col.textureXPos,0,1,WALL_TEXTURE_SZ };
					m_impl->wallTexture.render2(&textureClip, &col.rect);

					//// DRAW DISTANCE SHADOW
					//SDL_SetRenderDrawColor(global::instance.getRenderer(), col.color.r, col.color.g, col.color.b, col.color.a);
					//SDL_RenderFillRect(global::instance.getRenderer(), &col.rect);

					if (m_impl->showTopDown)
					{
						SDL_SetRenderDrawColor(global::instance.getRenderer(), 0, 200, 0, 0xFF);
						SDL_RenderDrawLine(global::instance.getRenderer(), col.ray.start.x, col.ray.start.y, col.ray.end.x, col.ray.end.y);
					}
				}
			}
			
#ifdef RENDER_DEBUG_VALUES
			{
				int yOffset = 0;
				const std::string strRenderTime = perfCounter.Stop();

				global::instance.renderMonospaceText("ray:" + strRayTime + " ms", SCREEN_WIDTH - 200, yOffset);
				global::instance.renderMonospaceText("drw:" + strRenderTime + " ms", SCREEN_WIDTH - 200, yOffset += 15);
				global::instance.renderMonospaceText("ren:" + m_impl->renderTime + " ms", SCREEN_WIDTH - 200, yOffset += 15);
				global::instance.renderMonospaceText("pix:" + std::to_string(numPixelsDrawn), SCREEN_WIDTH - 200, yOffset += 15);
				global::instance.renderMonospaceText("fps:" + m_impl->fps, SCREEN_WIDTH - 200, yOffset += 15);
			}
			
#endif
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

		{
			//SDL_Rect textureClip{ 0,0,32,64 };
			//SDL_Rect output{ 0,0,500,1000 };
			//m_impl->wallTexture.render2(&textureClip, &output);

		}
	}
	
	void GameSceneRaycaster::mouseDown(int button, int x, int y)
	{
		std::cout << "mouseDown: " << button << " " << x << " " << y << std::endl;

		// is the click inside the surface?

		if (x < m_impl->wallTexture.getWidth()
			&& y < m_impl->wallTexture.getHeight())
		{
			m_impl->testColor = m_impl->wallTexture.getPixelColor(x, y);
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
