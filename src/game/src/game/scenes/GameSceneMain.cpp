#include "GameSceneMain.h"
#include "GameSceneScaling.h"
#include "GameSceneRotating.h"
#include "GameSceneUserInput.h"
#include "GameSceneAABBCollision.h"
#include "GameScenePolygon.h"
#include "GameSceneMath.h"

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

		Pimpl()
			: optionsStrings{"0: Scaling", "1: Rotating", "2: User Input", "3: AABB Collision", "4: Polygon", "5: Math"},
			  bgTex{std::make_unique<rcgf::Texture>("img/dice.png")}
		{
			for (size_t i = 0; i < NUM_OPTIONS; ++i)
			{
				texArr[i] = std::make_unique<rcgf::Texture>(
					global::instance.getFont(), 
					optionsStrings[i].c_str()
				);
			}
		}
	};

	GameSceneMain::GameSceneMain()
		: m_impl{std::make_unique<Pimpl>()}
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
		case SDLK_0:
		case SDLK_KP_0:
			pushPendingState(std::make_unique<GameSceneScaling>());
			break;
		case SDLK_1:
		case SDLK_KP_1:
			pushPendingState(std::make_unique<GameStateRotating>());
			break;
		case SDLK_2:
		case SDLK_KP_2:
			pushPendingState(std::make_unique<GameSceneUserInput>());
			break;
		case SDLK_3:
		case SDLK_KP_3:
			pushPendingState(std::make_unique<GameSceneAABBCollision>());
			break;
		case SDLK_4:
		case SDLK_KP_4:
			pushPendingState(std::make_unique<GameScenePolygon>());
			break;
		case SDLK_5:
		case SDLK_KP_5:
			pushPendingState(std::make_unique<GameSceneMath>());
			break;
		}
	}
	
	void GameSceneMain::keyUp(SDL_Keycode)
	{

	}
}
