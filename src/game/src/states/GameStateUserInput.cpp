#include "GameStateUserInput.h"
#include "GameStateMain.h"
#include "glm/vec2.hpp"

namespace
{
	constexpr int NUM_OPTIONS = 5;
	constexpr float VELOCITY = 10.f;

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
	struct GameStateUserInput::Pimpl
	{
		std::array<std::string, 5> optionsStrings;
		std::array<std::unique_ptr<game::Texture>, NUM_OPTIONS> texArr;
		std::unique_ptr<game::Texture> movingTex;
		Scaler scaler;
		glm::vec2 pos;

		Pimpl() 
			: optionsStrings{"0: Back", "W: Up", "A: Left", "S: Down", "D: Right" }
			, movingTex{std::make_unique<game::Texture>("img/dice.png")}
			, pos{0.f,0.f}
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

	GameStateUserInput::GameStateUserInput()
		: m_impl{ std::make_unique<Pimpl>() }
	{}

	void GameStateUserInput::update()
	{
		m_impl->scaler.update();
	}

	void GameStateUserInput::render()
	{
		for (int i = 0; i < NUM_OPTIONS; ++i)
		{
			m_impl->texArr[i]->render(30, 30 + 30 * i);
		}
		const double angle = m_impl->scaler.getScale();
		const int w = m_impl->movingTex->getWidth();
		const int h = m_impl->movingTex->getHeight();

		m_impl->movingTex->render(
			static_cast<int>(m_impl->pos.x),
			static_cast<int>(m_impl->pos.y),
			NULL,
			1.f,
			angle
		);
	}

	void GameStateUserInput::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
			printf("switching to game state main from user input\n"); 
			pushPendingState(std::make_unique<GameStateMain>());
			break;
		case SDLK_w:
			m_impl->pos.y -= VELOCITY;
			break;
		case SDLK_a:
			m_impl->pos.x -= VELOCITY;
			break;
		case SDLK_s:
			m_impl->pos.y += VELOCITY;
			break;
		case SDLK_d:
			m_impl->pos.x += VELOCITY;
			break;
		}

	}

	void GameStateUserInput::keyUp(SDL_Keycode)
	{

	}
}
