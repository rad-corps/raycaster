#include "GameSceneMain.h"
#include "../raycaster/GameSceneRaycaster.h"

namespace
{
	constexpr int NUM_OPTIONS = 6;
}

namespace game
{
	struct GameSceneMain::Pimpl
	{
		const std::array<std::string, NUM_OPTIONS> optionsStrings;
		std::array<std::unique_ptr<rcgf::Texture>, NUM_OPTIONS> texArr;
		std::unique_ptr<rcgf::Texture> bgTex;

		Pimpl(SDL_Renderer* renderer, TTF_Font* font)
			: optionsStrings{"0: Transform", "1: User Input", "2: AABB Collision", "3: Polygon", "4: Math", "5: Raycasting"},
			  bgTex{std::make_unique<rcgf::Texture>(renderer, "img/dice.png")}
		{
			for (size_t i = 0; i < NUM_OPTIONS; ++i)
			{
				texArr[i] = std::make_unique<rcgf::Texture>(
					renderer,
					font, 
					optionsStrings[i].c_str()
				);
			}
		}

		Pimpl() = delete;
	};

	GameSceneMain::GameSceneMain(SDL_Renderer* renderer, TTF_Font* font)
		: m_impl{std::make_unique<Pimpl>(renderer, font)}
		, m_renderer{renderer}
		, m_font{font}
	{}

	void GameSceneMain::update()
	{
		
	}

	void GameSceneMain::render()
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

	void GameSceneMain::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_5:
		case SDLK_KP_5:
			pushPendingState(std::make_unique<GameSceneRaycaster>(m_renderer, m_font));
			break;
		}
	}
	
	void GameSceneMain::keyUp(SDL_Keycode)
	{

	}
}
