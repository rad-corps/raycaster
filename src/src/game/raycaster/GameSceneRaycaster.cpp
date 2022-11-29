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
#include "RenderEngine.h"


#define RENDER_DEBUG_VALUES

using math::PI;

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
		RenderEngine m_renderEngine;
		RaycastEngine raycastEngine;
		
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
			, enemySprite{ enemyAnimation.get(), math::Transform{math::Vec2{50.f, 70.f}, 0.f}}
			, m_renderer{ renderer }
			, m_renderEngine{ renderer }
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

	void GameSceneRaycaster::render()
	{
		// generate wall data
		const std::vector<ColumnRenderData>& crd = m_impl->raycastEngine.generateWallRenderData(m_impl->player.transform, &map, &m_impl->wallTexture);
		
		// render walls
		m_impl->m_renderEngine.RenderWalls(crd);

		// render sprites
		m_impl->m_renderEngine.RenderSprite(m_impl->player.transform, m_impl->enemySprite);

		if (m_impl->showTopDown)
		{
			m_impl->m_renderEngine.RenderTopDownMap(map, crd, m_impl->player.transform, m_impl->enemySprite.m_transform, m_impl->showRays);
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
