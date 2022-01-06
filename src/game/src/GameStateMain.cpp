#include "GameStateMain.h"

namespace game
{
	struct GameStateMain::Pimpl
	{

	};

	GameStateMain::GameStateMain(SDL_Renderer* renderer, TTF_Font* font)
		: m_optionsStrings {
			"0: Background Image",
			"1: Movement",
			"2: Rotation",
			"3: Collission"
		},
		m_bgTex{ std::make_unique<game::Texture>(renderer, "img/dice.png") },
		m_impl{std::make_unique<Pimpl>()}
	{	
		for (size_t i = 0; i < 4; ++i)
		{
			m_texArr[i] = std::make_unique<game::Texture>(renderer, font, m_optionsStrings[i].c_str());
		}
	}

	std::unique_ptr<IGameState> GameStateMain::update()
	{
		//Render current frame
		m_bgTex->render((SCREEN_WIDTH - m_bgTex->getWidth()) / 2, (SCREEN_HEIGHT - m_bgTex->getHeight()) / 2);

		for (int i = 0; i < 4; ++i)
		{
			m_texArr[i]->render(30, 30 + 30 * i);
		}

		return nullptr;
	}

	void GameStateMain::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
			printf("0 pressed\n"); break;
		case SDLK_1:
		case SDLK_KP_1:
			printf("1 pressed\n"); break;
		case SDLK_2:
		case SDLK_KP_2:
			printf("2 pressed\n"); break;
		case SDLK_3:
		case SDLK_KP_3:
			printf("3 pressed\n"); break;
		}
		
	}
	
	void GameStateMain::keyUp(SDL_Keycode)
	{

	}
}
