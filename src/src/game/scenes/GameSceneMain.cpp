#include "GameSceneMain.h"
#include "GameSceneRaycaster.h"
#include "Map.h"

#include "SDL_gamecontroller.h"

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

		// render the map name to text
		if (m_capturingMapName)
		{
			global::Global::renderMonospaceText("Enter Map Name:", 20, 180);
		}
		global::Global::renderMonospaceText(m_mapTempName, 20, 200);
	}

	void GameSceneMain::keyDownCapturingText(SDL_Keycode keycode)
	{
		if (keycode == SDLK_RETURN)
		{
			const std::string mapName = "./rooms/out/" + m_mapTempName;
			pushPendingState(std::make_unique<GameSceneRaycaster>(m_renderer, m_font, mapName));
			m_capturingMapName = false;
			return;
		}

		if (keycode >= SDLK_SPACE && keycode <= SDLK_z)
		{
			m_mapTempName += (char)keycode;
			return;
		}

		if (keycode == SDLK_BACKSPACE)
		{
			m_mapTempName.pop_back();
		}
	}
	void GameSceneMain::keyDownImpl(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
			pushPendingState(std::make_unique<GameSceneRaycaster>(m_renderer, m_font, m_mapName));
			break;
		case SDLK_1:
		case SDLK_KP_1:
			m_capturingMapName = true;
			break;
		}
	}

	void GameSceneMain::keyDown(SDL_Keycode keycode)
	{
		if (m_capturingMapName)
		{
			keyDownCapturingText(keycode);
		}
		else
		{
			keyDownImpl(keycode);
		}
	}
	
	void GameSceneMain::keyUp(SDL_Keycode)
	{

	}
}
