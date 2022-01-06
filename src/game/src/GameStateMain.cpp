#include "GameStateMain.h"
#include "GameStateMoving.h"

namespace game
{
	struct GameStateMain::Pimpl
	{
		const std::array<std::string, 4> m_optionsStrings;
		std::array<std::unique_ptr<game::Texture>, 4> m_texArr;
		std::unique_ptr<game::Texture> m_bgTex;
		std::unique_ptr<game::IGameState> nextState;
		SDL_Renderer* renderer;
		TTF_Font* font;

		Pimpl(SDL_Renderer* renderer_, TTF_Font* font_)
			: m_optionsStrings{
				"0: Background Image",
				"1: Movement",
				"2: Rotation",
				"3: Collission"
			},
			m_bgTex{ 
				std::make_unique<game::Texture>(renderer_, "img/dice.png")
			},
			nextState{nullptr},
			renderer{ renderer_ },
			font{ font_ }
		{
			for (size_t i = 0; i < 4; ++i)
			{
				m_texArr[i] = std::make_unique<game::Texture>(renderer_, font, m_optionsStrings[i].c_str());
			}
		}
	};

	GameStateMain::GameStateMain(SDL_Renderer* renderer, TTF_Font* font)
		: m_impl{std::make_unique<Pimpl>(renderer, font)}
	{}

	std::unique_ptr<IGameState> GameStateMain::update()
	{
		if (m_impl->nextState)
		{
			std::unique_ptr<IGameState> tempState = std::move(m_impl->nextState);
			m_impl->nextState.reset(nullptr);
			return tempState;
		}
		return nullptr;
	}

	void GameStateMain::render()
	{
		//Render current frame
		m_impl->m_bgTex->render(
			(SCREEN_WIDTH - m_impl->m_bgTex->getWidth()) / 2,
			(SCREEN_HEIGHT - m_impl->m_bgTex->getHeight()) / 2
		);

		for (int i = 0; i < 4; ++i)
		{
			m_impl->m_texArr[i]->render(30, 30 + 30 * i);
		}
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
			m_impl->nextState = std::make_unique<GameStateMoving>(m_impl->renderer, m_impl->font);
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
