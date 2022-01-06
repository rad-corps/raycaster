#include "GameStateMain.h"
#include "GameStateScaling.h"
#include "GameStateRotating.h"

namespace
{
	constexpr int NUM_OPTIONS = 2;
}

namespace game
{
	struct GameStateMain::Pimpl
	{
		const std::array<std::string, 4> optionsStrings;
		std::array<std::unique_ptr<game::Texture>, NUM_OPTIONS> texArr;
		std::unique_ptr<game::Texture> bgTex;

		Pimpl()
			: optionsStrings{"0: Scaling", "1: Rotating"},
			  bgTex{std::make_unique<game::Texture>("img/dice.png")}
		{
			for (size_t i = 0; i < NUM_OPTIONS; ++i)
			{
				texArr[i] = std::make_unique<game::Texture>(
					global::instance.getFont(), 
					optionsStrings[i].c_str()
				);
			}
		}
	};

	GameStateMain::GameStateMain()
		: m_impl{std::make_unique<Pimpl>()}
	{}

	void GameStateMain::update()
	{
		
	}

	void GameStateMain::render()
	{
		//Render current frame
		m_impl->bgTex->render(
			(SCREEN_WIDTH - m_impl->bgTex->getWidth()) / 2,
			(SCREEN_HEIGHT - m_impl->bgTex->getHeight()) / 2
		);

		for (int i = 0; i < NUM_OPTIONS; ++i)
		{
			m_impl->texArr[i]->render(30, 30 + 30 * i);
		}
	}

	void GameStateMain::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
			pushPendingState(std::make_unique<GameStateScaling>());
			printf("0 pressed\n"); break;
		case SDLK_1:
		case SDLK_KP_1:
			pushPendingState(std::make_unique<GameStateRotating>());
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
