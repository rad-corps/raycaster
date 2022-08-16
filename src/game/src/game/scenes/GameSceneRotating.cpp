#include "GameSceneRotating.h"
#include "GameSceneMain.h"

namespace
{
	constexpr int NUM_OPTIONS = 1;

	class Scaler
	{
	private:
		double m_scale = 0.0;
	
	public:

		void update()
		{
			m_scale += 0.1;
		}
		double getScale()
		{
			return m_scale;
		}
	};
}

namespace game
{
	struct GameStateRotating::Pimpl
	{
		std::array<std::string, 1> optionsStrings;
		std::array<std::unique_ptr<rcgf::Texture>, NUM_OPTIONS> texArr;
		std::unique_ptr<rcgf::Texture> movingTex;
		Scaler scaler;

		Pimpl() 
			: optionsStrings{"0: Back"}
			, movingTex{std::make_unique<rcgf::Texture>("img/dice.png")}			
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

	GameStateRotating::GameStateRotating()
		: m_impl{ std::make_unique<Pimpl>() }
	{}

	void GameStateRotating::update()
	{
		m_impl->scaler.update();
	}

	void GameStateRotating::render()
	{
		for (int i = 0; i < NUM_OPTIONS; ++i)
		{
			m_impl->texArr[i]->render(30, 30 + 30 * i);
		}
		const double angle = m_impl->scaler.getScale();
		const int w = m_impl->movingTex->getWidth();
		const int h = m_impl->movingTex->getHeight();

		m_impl->movingTex->render(
			(SCREEN_WIDTH - w) / 2,
			(SCREEN_HEIGHT - h) / 2,
			NULL,
			1.f,
			angle
		);
	}

	void GameStateRotating::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
		case SDLK_ESCAPE:
			printf("switching to game state main\n"); 
			pushPendingState(std::make_unique<GameSceneMain>());
			break;
		}

	}

	void GameStateRotating::keyUp(SDL_Keycode)
	{

	}
}
