#include "GameSceneScaling.h"
#include "GameSceneMain.h"

namespace
{
	constexpr int NUM_OPTIONS = 1;

	class Scaler
	{
	private:
		enum class Direction
		{
			INCREASE,
			DECREASE
		};

		float m_scale = 1.f;
		Direction m_dir = Direction::DECREASE;

	
	public:

		void update()
		{
			if (m_dir == Direction::INCREASE)
			{
				m_scale += 0.01f;

				if (m_scale > 1.f)
				{
					m_dir = Direction::DECREASE;
				}
			}
			else
			{
				m_scale -= 0.01f;

				if (m_scale < 0.f)
				{
					m_dir = Direction::INCREASE;
				}
			}
		}
		float getScale()
		{
			return m_scale;
		}
	};
}

namespace game
{
	struct GameSceneScaling::Pimpl
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

	GameSceneScaling::GameSceneScaling()
		: m_impl{ std::make_unique<Pimpl>() }
	{}

	void GameSceneScaling::update()
	{
		m_impl->scaler.update();
	}

	void GameSceneScaling::render()
	{
		for (int i = 0; i < NUM_OPTIONS; ++i)
		{
			m_impl->texArr[i]->render(30, 30 + 30 * i);
		}
		const float scale = m_impl->scaler.getScale();
		const int w = static_cast<int>(m_impl->movingTex->getWidth() * scale);
		const int h = static_cast<int>(m_impl->movingTex->getHeight() * scale);

		m_impl->movingTex->render(
			(SCREEN_WIDTH - w) / 2,
			(SCREEN_HEIGHT - h) / 2,
			NULL,
			scale,
			0.0
		);
	}

	void GameSceneScaling::keyDown(SDL_Keycode keycode)
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

	void GameSceneScaling::keyUp(SDL_Keycode)
	{

	}
}
