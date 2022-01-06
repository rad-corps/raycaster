#include "GameStateMoving.h"
#include "GameStateMain.h"

namespace game
{
	struct GameStateMoving::Pimpl
	{
		const std::array<std::string, 1> m_optionsStrings;
		std::array<std::unique_ptr<game::Texture>, 1> m_texArr;
		std::unique_ptr<game::IGameState> nextState;
		SDL_Renderer* renderer; 
		TTF_Font* font;

		Pimpl(SDL_Renderer* renderer_, TTF_Font* font_)
			: m_optionsStrings{
				"0: Back"
			},
			nextState{ nullptr },
			renderer{renderer_},
			font{font_}
		{
			for (size_t i = 0; i < 1; ++i)
			{
				m_texArr[i] = std::make_unique<game::Texture>(renderer_, font, m_optionsStrings[i].c_str());
			}
		}
	};

	GameStateMoving::GameStateMoving(SDL_Renderer* renderer, TTF_Font* font)
		: m_impl{ std::make_unique<Pimpl>(renderer, font) }
	{}

	std::unique_ptr<IGameState> GameStateMoving::update()
	{
		if (m_impl->nextState)
		{
			std::unique_ptr<IGameState> tempState = std::move(m_impl->nextState);
			m_impl->nextState.reset(nullptr);
			return tempState;
		}
		return nullptr;
	}

	void GameStateMoving::render()
	{
		for (int i = 0; i < 1; ++i)
		{
			m_impl->m_texArr[i]->render(30, 30 + 30 * i);
		}
	}

	void GameStateMoving::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
			printf("switching to game state main\n"); 
			m_impl->nextState = std::make_unique<GameStateMain>(m_impl->renderer, m_impl->font);
			break;
		}

	}

	void GameStateMoving::keyUp(SDL_Keycode)
	{

	}
}
