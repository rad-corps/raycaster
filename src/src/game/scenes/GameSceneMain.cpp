#include "GameSceneMain.h"
#include "GameSceneRaycaster.h"
#include "Map.h"

namespace
{
	constexpr int NUM_OPTIONS = 2;
}

namespace game
{
	struct GameSceneMain::Pimpl
	{
		const std::array<std::string, NUM_OPTIONS> optionsStrings;
		std::array<std::unique_ptr<rcgf::Texture>, NUM_OPTIONS> texArr;
		std::unique_ptr<rcgf::Texture> bgTex;

		Pimpl(SDL_Renderer* renderer, TTF_Font* font)
			: optionsStrings{"0: Raycasting", "1: Load Map"},
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

	void GameSceneMain::render(double deltatime)
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
		case SDLK_0:
		case SDLK_KP_0:
			pushPendingState(std::make_unique<GameSceneRaycaster>(m_renderer, m_font, m_mapName));
			break;
		case SDLK_1:
		case SDLK_KP_1:
			std::cout << "Load Map" << std::endl;
			std::string fname;
			std::cout << "enter filename (leave blank for default.csv): " << fname;
			std::getline(std::cin, fname);
			if (fname.empty())
			{
				fname = "default.csv";
			}
			m_mapName = "./rooms/" + fname;
			break;
		}
	}
	
	void GameSceneMain::keyUp(SDL_Keycode)
	{

	}
}
