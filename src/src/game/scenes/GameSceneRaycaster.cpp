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
#include "./gameobjects/base/GameObject.h"
#include "./gameobjects/base/GameObjectPool.h"
#include "RaycasterConstants.h"
#include <vector>
#include "RenderEngine.h"
#include "./gameobjects/factory/GameObjectFactory.h"

#include <algorithm> // std::sort

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "EventSystem.h"
#include "Spatial.h"


#define RENDER_DEBUG_VALUES

using math::PI;

namespace
{
	SimplePerfCounter perfCounter;
}

namespace game
{
	struct GameSceneRaycaster::Pimpl
	{
		Player player;
		bool showTopDown = false;
		bool showRays = false;
		rcgf::Texture wallTexture;
		SDL_Renderer* m_renderer;
		RaycastEngine raycastEngine;
		RenderEngine m_renderEngine;
		GameObjectPool m_gameObjects;
		//std::vector<GameObject> m_gameObjects;

		std::map<SDL_Keycode, bool> keyStates = {
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
			: player{ math::Transform{58.4994f, 149.201f, 0.0299706f} }
			, wallTexture{ renderer, "./img/wall_64.png" }
			, m_renderer{ renderer }
			, m_renderEngine{ renderer, raycastEngine.GetColumnRenderData() }
			, m_gameObjects(GAME_OBJECT_ACTIVE_POOL_SZ, m_renderEngine)
		{
			srand((unsigned int)time(NULL));
			
			// TODO: cleanup texture memory on exit
			factory::Init(m_renderer);
			spatial::init_path_finding();

			// initialise level by adding an enemy with a waypoint path
			GameObject cabron = factory::CreateCabron(
				{ 228.849f, 151.799f, 3.15316f }, 
				{
					{223.661f, 113.27f},
					{169.576f, 107.703f},
					{142.766f, 44.0217f},
					{99.6444f, 89.271f},
					{99.5096f, 146.271f},
					{38.0722f, 155.339f},
					{43.3761f, 211.787f},
					{117.929f, 217.882f},
					{175.284f, 235.681f},
					{184.17f, 212.622f},
					{226.004f, 151.912f}
				}
			);
			m_gameObjects.Add(std::move(cabron));
		}
		Pimpl() = delete;
		~Pimpl()
		{
			events::clear();
		}
	};


	GameSceneRaycaster::GameSceneRaycaster(SDL_Renderer* renderer, TTF_Font* font)
		: m_impl{ std::make_unique<Pimpl>(renderer) }
		, m_renderer{ renderer }
		, m_font{ font }
	{}

	void GameSceneRaycaster::update()
	{
		
	}

	void GameSceneRaycaster::fixedUpdate()
	{
		m_impl->m_renderEngine.ClearTopDownMapData();

		Player& player = m_impl->player;
		auto& keyStates = m_impl->keyStates;

		m_impl->m_gameObjects.Update(map::get_map(), { player.transform });

		if (!keyStates[SDLK_LCTRL])
		{
			// rotate
			if (keyStates[SDLK_d] || keyStates[SDLK_RIGHT]) player.rotate(Player::RotateDirection::Clockwise);
			if (keyStates[SDLK_a] || keyStates[SDLK_LEFT])  player.rotate(Player::RotateDirection::Anticlockwise);
		}
		else
		{
			// strafe
			if (keyStates[SDLK_d] || keyStates[SDLK_RIGHT]) player.move(PI / 2.f, &map::get_map());
			if (keyStates[SDLK_a] || keyStates[SDLK_LEFT])  player.move(PI + PI * 0.5f, &map::get_map());
		}

		// forward and backward movement
		if (keyStates[SDLK_w] || keyStates[SDLK_UP])    player.move(0.f, &map::get_map());
		if (keyStates[SDLK_s] || keyStates[SDLK_DOWN])  player.move(PI, &map::get_map());
	}

	void GameSceneRaycaster::render(double deltatime)
	{
		const math::Transform& playerTransform = m_impl->player.transform;
		auto& gameObjects = m_impl->m_gameObjects;
		auto& renderEngine = m_impl->m_renderEngine;
		
		// generate wall data
		m_impl->raycastEngine.generateWallRenderData(playerTransform, &map::get_map(), &m_impl->wallTexture);

		// render walls
		m_impl->m_renderEngine.RenderWalls();

		// render all gameobjects
		gameObjects.Render(playerTransform, deltatime);
		renderEngine.RenderTopDownMap(map::get_map(), playerTransform, m_impl->showRays);
	}

	void GameSceneRaycaster::keyDown(SDL_Keycode keycode)
	{
		auto& keyStates = m_impl->keyStates;

		keyStates[keycode] = true;

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
			keyStates[keycode] = true;
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
			m_impl->m_renderEngine.SetTopDownMapActive(m_impl->showTopDown);
			break;
		case SDLK_BACKQUOTE:
			if (m_impl->showTopDown)
			{
				m_impl->showRays = !m_impl->showRays;
			}
			break;
		case SDLK_SPACE:
		{
			const math::Transform& t = m_impl->player.transform;
			std::cout << "{" << t.pos.x << "f, " << t.pos.y << "f, " << t.angle << "f}" << std::endl;
			break;
		}
		case SDLK_LALT:
			m_impl->m_gameObjects.Add(factory::CreatePlayerBullet(m_impl->player.transform, nullptr));
			break;
		case SDLK_RETURN:
			std::vector<GameObject>& gameObjects = m_impl->m_gameObjects.GetPool();
			for (auto& gameObject : gameObjects)
			{
				gameObject.SendAlert(m_impl->player.transform.pos);
			}
			break;

		}


	}

	void GameSceneRaycaster::keyUp(SDL_Keycode keycode)
	{
		m_impl->keyStates[keycode] = false;
	}
}
