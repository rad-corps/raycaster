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
		bool show3D = true;
		rcgf::Texture wallTexture{ "./img/wall_64.png" };
		
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

				// cast the rays and render to screen
				crd = raycast_engine::doRayTest(m_impl->player.transform, finalAngle, screenColumnNumber, &map);
			}

#ifdef RENDER_DEBUG_VALUES
			const std::string strRayTime = perfCounter.Stop();
			perfCounter.Start();
#endif

			// draw floors
			{

				//for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; y += X_PX_STEP)
				for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; y++)
				{
					// get direction vector from player
					const Vec2 playerAngle = angleToVec(m_impl->player.transform.angle);
					const float& dirX = playerAngle.x;
					const float& dirY = playerAngle.y;
					const float& planeX = m_impl->player.plane.x;
					const float& planeY = m_impl->player.plane.y;
					const float& posX = m_impl->player.transform.x;
					const float& posY = m_impl->player.transform.y;

					float rayDirX0 = dirX - planeX;
					float rayDirY0 = dirY - planeY;
					float rayDirX1 = dirX + planeX;
					float rayDirY1 = dirY + planeY;

					// Vertical position of the camera.
					// NOTE: with 0.5, it's exactly in the center between floor and ceiling,
					// matching also how the walls are being raycasted. For different values
					// than 0.5, a separate loop must be done for ceiling and floor since
					// they're no longer symmetrical.
					const float posZ = 0.5 * SCREEN_HEIGHT;

					// Current y position compared to the center of the screen (the horizon)
					const int p = y - SCREEN_HEIGHT / 2;

					// Horizontal distance from the camera to the floor for the current row.
					// 0.5 is the z position exactly in the middle between floor and ceiling.
					// NOTE: this is affine texture mapping, which is not perspective correct
					// except for perfectly horizontal and vertical surfaces like the floor.
					// NOTE: this formula is explained as follows: The camera ray goes through
					// the following two points: the camera itself, which is at a certain
					// height (posZ), and a point in front of the camera (through an imagined
					// vertical plane containing the screen pixels) with horizontal distance
					// 1 from the camera, and vertical position p lower than posZ (posZ - p). When going
					// through that point, the line has vertically traveled by p units and
					// horizontally by 1 unit. To hit the floor, it instead needs to travel by
					// posZ units. It will travel the same ratio horizontally. The ratio was
					// 1 / p for going through the camera plane, so to go posZ times farther
					// to reach the floor, we get that the total horizontal distance is posZ / p.
					const float rowDistance = posZ / p;

					// calculate the real world step vector we have to add for each x (parallel to camera plane)
					// adding step by step avoids multiplications with a weight in the inner loop
					float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
					float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

					// real world coordinates of the leftmost column. This will be updated as we step to the right.
					float floorX = posX + rowDistance * rayDirX0;
					float floorY = posY + rowDistance * rayDirY0;

					//for (int x = 0; x < SCREEN_WIDTH; x += X_PX_STEP)
					for (int x = 0; x < SCREEN_WIDTH; x ++)
					{
						// the cell coord is simply got from the integer parts of floorX and floorY
						const int cellX = (int)(floorX / WALL_TEXTURE_SZ);
						const int cellY = (int)(floorY / WALL_TEXTURE_SZ);

						// get the texture coordinate from the fractional part
						int tx = (int)(WALL_TEXTURE_W * (floorX - cellX)) & (WALL_TEXTURE_W - 1);
						int ty = (int)(WALL_TEXTURE_H * (floorY - cellY)) & (WALL_TEXTURE_H - 1);

						floorX += floorStepX;
						floorY += floorStepY;

						// floor
						Color color = m_impl->wallTexture.getPixelColor(ty, tx);
						SDL_SetRenderDrawColor(global::instance.getRenderer(), color.r, color.g, color.b, 0xFF);
						SDL_Rect r{ x,y,X_PX_STEP,X_PX_STEP };
						SDL_RenderFillRect(global::instance.getRenderer(), &r);
					}
				}

			} // end draw floors

			// draw the 3d scene
			//SDL_SetRenderDrawColor(global::instance.getRenderer(), 0, 200, 0, 0xFF);
			if (m_impl->show3D)
			{
				for (const ColumnRenderData& col : m_impl->columnRenderDataArray)
				{
					// PAINT ONLY COLUMNS
					const SDL_Rect textureClip{ col.textureXPos,0,1,WALL_TEXTURE_SZ };
					m_impl->wallTexture.render2(&textureClip, &col.rect);
				}
			}

#ifdef RENDER_DEBUG_VALUES
			{
				int yOffset = 0;
				const std::string strRenderTime = perfCounter.Stop();

				global::instance.renderMonospaceText("ray:" + strRayTime + " ms", SCREEN_WIDTH - 200, yOffset);
				global::instance.renderMonospaceText("drw:" + strRenderTime + " ms", SCREEN_WIDTH - 200, yOffset += 15);
				global::instance.renderMonospaceText("ren:" + m_impl->renderTime + " ms", SCREEN_WIDTH - 200, yOffset += 15);
				global::instance.renderMonospaceText("fps:" + m_impl->fps, SCREEN_WIDTH - 200, yOffset += 15);
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
