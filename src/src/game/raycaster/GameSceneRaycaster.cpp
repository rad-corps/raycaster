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
#include "Actor.h"
#include "RaycasterConstants.h"
#include <vector>
#include "RenderEngine.h"

#include <algorithm> // std::sort

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "AI.h"


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
		rcgf::Texture bulletTexture;
		std::unique_ptr<rcgf::Animation> enemyAnimation;
		SDL_Renderer* m_renderer;
		RaycastEngine raycastEngine;
		RenderEngine m_renderEngine;
		//std::vector<game::Actor> spriteArray;
		std::vector<PlayerBullet> playerBullets;
		game::Actor testSprite;

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
			, bulletTexture{ renderer, "./img/player_bullet.png"}
			, enemyAnimation{ std::make_unique<rcgf::Animation>(
					std::make_unique<rcgf::Texture>(renderer, "img/CabronTileset.png"),
					64, // sprite width
					64, // sprite height
					1,  // rows
					4  // cols
				) }
			, m_renderer{ renderer }
			, m_renderEngine{ renderer, raycastEngine.GetColumnRenderData() }
			, testSprite{
				enemyAnimation.get(),
				math::Transform{92.7399f, 150.433f, 0.f},
				std::make_unique<AI_WaypointFollow>(std::vector<math::Vec2>
					{
						math::Vec2{139.675f, 162.776f},
						math::Vec2{116.09f, 140.828f},
						math::Vec2{106.411f, 73.3184f},
						math::Vec2{81.0254f, 139.27f},
						math::Vec2{50.7314f, 134.953f},
						math::Vec2{49.8596f, 79.302f},
						math::Vec2{30.3543f, 84.3723f},
						math::Vec2{61.1379f, 156.247f},
						math::Vec2{100.179f, 158.618f}
					}
				) }
		{
			srand((unsigned int)time(NULL));
		}
		Pimpl() = delete;
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
		Player& player = m_impl->player;
		auto& keyStates = m_impl->keyStates;

		m_impl->testSprite.Update();

		for (auto& bullet : m_impl->playerBullets)
		{
			bullet.update();
		}

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
		m_impl->raycastEngine.generateWallRenderData(m_impl->player.transform, &map, &m_impl->wallTexture);

		// render walls
		m_impl->m_renderEngine.RenderWalls();

		// sort sprites (closest to player last)
		const math::Vec2 player_pos = m_impl->player.transform.pos;

		//std::sort(m_impl->spriteArray.begin(), m_impl->spriteArray.end(), [player_pos](const Actor& a, const Actor& b) {
		//	return math::magnitude(player_pos - a.m_transform.pos) > math::magnitude(player_pos - b.m_transform.pos);
		//});
		//// render sprites
		//for (const auto& sprite : m_impl->spriteArray)
		//{
		//	m_impl->m_renderEngine.RenderSprite(m_impl->player.transform, sprite);
		//}

		m_impl->m_renderEngine.RenderSprite(m_impl->player.transform, m_impl->testSprite);
		for (auto& bullet : m_impl->playerBullets)
		{
			m_impl->m_renderEngine.RenderSprite(m_impl->player.transform, bullet.transform, m_impl->bulletTexture);
		}
		

		if (m_impl->showTopDown)
		{
			m_impl->m_renderEngine.RenderTopDownMap(map, m_impl->player.transform, m_impl->showRays);
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
		case SDLK_SPACE:
		{
			const math::Transform& t = m_impl->player.transform;
			std::cout << "{" << t.pos.x << "f, " << t.pos.y << "f, " << t.angle << "f}" << std::endl;
			break;
		}
		case SDLK_LALT:
			// todo: 
			// make this a pool
			// clean up when colliding with wall or enemy
			m_impl->playerBullets.push_back(PlayerBullet{ m_impl->player.transform });
			break;
		}

	}

	void GameSceneRaycaster::keyUp(SDL_Keycode keycode)
	{
		m_impl->keyStates[keycode] = false;
	}
}
