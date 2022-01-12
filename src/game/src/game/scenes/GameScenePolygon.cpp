#include "GameScenePolygon.h"
#include "GameSceneMain.h"
#include "Collision.h"
#include "Globals.h"

namespace
{
}

namespace game
{
	struct GameScenePolygon::Pimpl
	{
		Pimpl() 
		{
		}
	};

	GameScenePolygon::GameScenePolygon()
		: m_impl{ std::make_unique<Pimpl>() }
	{}

	void GameScenePolygon::update()
	{
	}

	void GameScenePolygon::render()
	{
		SDL_Point points[5];
		points[0].x = 10;
		points[0].y = 10;
		points[1].x = 110;
		points[1].y = 10;
		points[2].x = 110;
		points[2].y = 110;
		points[3].x = 10;
		points[3].y = 110;
		points[4].x = 10;
		points[4].y = 10;
		SDL_SetRenderDrawColor(global::instance.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawLines(global::instance.getRenderer(), points, 5);

		global::instance.renderMonospaceText("HELLO WORLD 0123456789!", 100, 100);
	}

	void GameScenePolygon::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
			printf("switching to game state main\n"); 
			pushPendingState(std::make_unique<GameSceneMain>());
			break;
		}

	}

	void GameScenePolygon::keyUp(SDL_Keycode)
	{

	}
}
