#include "GameScenePolygon.h"
#include "GameSceneMain.h"
#include "Collision.h"
#include "Globals.h"
#include "Polygon.h"

namespace
{
}

namespace game
{
	struct GameScenePolygon::Pimpl
	{
		std::unique_ptr<rcgf::Square> square;
		Pimpl(SDL_Renderer* renderer) 
			: square{std::make_unique<rcgf::Square>(renderer, 100)}
		{
		}
	};

	GameScenePolygon::GameScenePolygon(SDL_Renderer* renderer, TTF_Font* font)
		: m_impl{ std::make_unique<Pimpl>(renderer) }
		, m_renderer{renderer}
		, m_font{ font }
	{}

	void GameScenePolygon::update()
	{
	}

	void GameScenePolygon::render(SDL_Renderer* renderer)
	{
		// 4100 
		renderer;

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
		SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawLines(m_renderer, points, 5);

		global::Global::renderMonospaceText("HELLO WORLD 0123456789!", 100, 100);

		m_impl->square->render(math::Vec2f(200,220));
	}

	void GameScenePolygon::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
		case SDLK_ESCAPE:
			printf("switching to game state main\n"); 
			pushPendingState(std::make_unique<GameSceneMain>(m_renderer, m_font));
			break;
		}

	}

	void GameScenePolygon::keyUp(SDL_Keycode)
	{

	}
}
