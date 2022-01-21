#include "GameSceneScaling.h"
#include "GameSceneMain.h"

namespace
{
	constexpr int NUM_OPTIONS = 3;

	class Rotator
	{
	private:
		double m_rot = 0.0;
		bool active = false;
	
	public:
		void toggleActivate()
		{
			active = !active;
		}
		void update()
		{
			if (!active) return;
			m_rot += 0.5;
		}
		double getRotation()
		{
			return m_rot;
		}
	};

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
		bool active = false;

	public:

		void toggleActivate()
		{
			active = !active;
		}

		void update()
		{
			if ( !active ) return;

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
		std::array<std::string, NUM_OPTIONS> optionsStrings;
		std::array<std::unique_ptr<rcgf::Texture>, NUM_OPTIONS> texArr;
		std::unique_ptr<rcgf::Texture> movingTex;
		Scaler scaler;
		Rotator rotator;

		Pimpl() 
			: optionsStrings{"0: Back", "1: Scaling", "2: Rotation"}
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
		m_impl->rotator.update();
	}

	void GameSceneScaling::render()
	{
		for (int i = 0; i < NUM_OPTIONS; ++i)
		{
			m_impl->texArr[i]->render(30, 30 + 30 * i);
		}
		const float scale = m_impl->scaler.getScale();
		const double angle = m_impl->rotator.getRotation();
		const int w = static_cast<int>(m_impl->movingTex->getWidth() * scale);
		const int h = static_cast<int>(m_impl->movingTex->getHeight() * scale);

		m_impl->movingTex->render(
			(SCREEN_WIDTH - w) / 2,
			(SCREEN_HEIGHT - h) / 2,
			NULL,
			scale,
			angle
		);
	}

	void GameSceneScaling::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
			printf("switching to game state main\n"); 
			pushPendingState(std::make_unique<GameSceneMain>());
			break;
		case SDLK_KP_1:
		case SDLK_1:
			m_impl->scaler.toggleActivate();
			break;
		case SDLK_KP_2:
		case SDLK_2:
			m_impl->rotator.toggleActivate();
			break;
		}

	}

	void GameSceneScaling::keyUp(SDL_Keycode)
	{

	}
}
