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
#include "Sprite.h"
#include "RaycasterConstants.h"
#include <vector>


#define RENDER_DEBUG_VALUES

namespace
{
	SimplePerfCounter perfCounter;

	// TODO: move this to Map.h or Map.cpp
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
		bool showRays = false;
		rcgf::Texture wallTexture;
		std::unique_ptr<rcgf::Animation> enemyAnimation;
		game::Sprite enemySprite;
		SDL_Renderer* m_renderer;
		
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
			: wallTexture{ renderer, "./img/wall_64.png" }
			, enemyAnimation{std::make_unique<rcgf::Animation>(
					std::make_unique<rcgf::Texture>(renderer, "img/Cabron_360.png"),
					64, // sprite width
					64, // sprite height
					1,  // rows
					4  // cols
				)
			}
			, enemySprite{ renderer, enemyAnimation.get(), math::Transform{math::Vec2{50.f, 70.f}, 0.f}}
			, m_renderer{ renderer }
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

	void GameSceneRaycaster::fixedUpdate()
	{
		Player& player = m_impl->player;
		auto& keyStates = m_impl->keyStates;

		if (!keyStates[SDLK_LCTRL])
		{
			// rotate
			if (keyStates[SDLK_d] || keyStates[SDLK_RIGHT]) player.rotate(Player::RotateDirection::Clockwise);
			if (keyStates[SDLK_a] || keyStates[SDLK_LEFT])  player.rotate(Player::RotateDirection::Anticlockwise);
		}
		else
		{
			// strafe
			if (keyStates[SDLK_d] || keyStates[SDLK_RIGHT]) player.move(PI / 2.f, &map);
			if (keyStates[SDLK_a] || keyStates[SDLK_LEFT])  player.move(PI + PI * 0.5f, &map);
		}
		
		// forward and backward movement
		if (keyStates[SDLK_w] || keyStates[SDLK_UP])    player.move(0.f, &map);
		if (keyStates[SDLK_s] || keyStates[SDLK_DOWN])  player.move(PI, &map);
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
				if (m_impl->showRays)
				{
					SDL_SetRenderDrawColor(m_renderer, 133, 133, 173, 255);
					SDL_RenderDrawLine(m_renderer, crd.ray.start.x * TOP_DOWN_SCALE, crd.ray.start.y * TOP_DOWN_SCALE, crd.ray.end.x * TOP_DOWN_SCALE, crd.ray.end.y * TOP_DOWN_SCALE);
				}

				// draw walls
				const SDL_Rect textureClip{ crd.textureXPos,0,1,WALL_TEXTURE_SZ };
				m_impl->wallTexture.render2(&textureClip, &crd.rect);

#ifdef RENDER_FLOORS
				raycast_engine::drawFloorColumn(m_renderer, m_impl->player.transform, crd, screenColumnNumber, rayAngle, &m_impl->wallTexture);
#endif
			}

#ifdef RENDER_DEBUG_VALUES
			const std::string strRayTime = perfCounter.Stop();
			perfCounter.Start();
#endif
		}

		// draw the map
		if (m_impl->showTopDown)
		{
			SDL_SetRenderDrawColor(m_renderer, 0, 128, 0, 0xFF);
			for (int row = 0; row < MAP_ROWS; ++row)
			{
				for (int col = 0; col < MAP_COLS; ++col)
				{
					// calc rect position and dimension
					SDL_Rect rect{ col * MAP_CELL_PX * TOP_DOWN_SCALE, row * MAP_CELL_PX * TOP_DOWN_SCALE, MAP_CELL_PX * TOP_DOWN_SCALE, MAP_CELL_PX * TOP_DOWN_SCALE };
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


			// TODO: move this to Sprite::render
			//SDL_Rect enemyRect{
			//	(int)m_impl->enemyPos.x * TOP_DOWN_SCALE - 2,
			//	(int)m_impl->enemyPos.y * TOP_DOWN_SCALE - 2,
			//	4, 4
			//};
			//SDL_SetRenderDrawColor(m_renderer, 255, 0, 100, 0xFF);
			//SDL_RenderFillRect(m_renderer, &enemyRect);
		}

		m_impl->enemySprite.render(m_impl->player.transform, m_impl->showTopDown);
	}
	
	void GameSceneRaycaster::mouseDown(int button, int x, int y)
	{
		button; x; y;
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
			m_impl->showRays = m_impl->showTopDown;
			break;
		case SDLK_BACKQUOTE:
			if (m_impl->showTopDown)
			{
				m_impl->showRays = !m_impl->showRays;
			}
			break;
		}

	}

	void GameSceneRaycaster::keyUp(SDL_Keycode keycode)
	{
		m_impl->keyStates[keycode] = false;
	}
}
