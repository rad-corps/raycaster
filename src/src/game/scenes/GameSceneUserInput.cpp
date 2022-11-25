#include "GameSceneUserInput.h"
#include "GameSceneMain.h"
#include "Animation.h"
#include <sstream>
#include "RCGFMath.h"


namespace
{
	constexpr int NUM_OPTIONS = 5;
	constexpr float VELOCITY = 10.f;

	constexpr int SPRITE_H = 59;
	constexpr int SPRITE_W = 62;

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
	struct GameSceneUserInput::Pimpl
	{
		std::array<std::string, 5> optionsStrings;
		std::array<std::unique_ptr<rcgf::Texture>, NUM_OPTIONS> texArr;
		std::unique_ptr<rcgf::Texture> movingTex;
		std::unique_ptr<rcgf::Animation> animation;
		int animIdx = 0;
		int animCounter = 0;
		Scaler scaler;
		math::Vec2 pos;
		math::Vec2 velocity;

		Pimpl(SDL_Renderer* renderer, TTF_Font* font)
			: optionsStrings{"0: Back", "W: Up", "A: Left", "S: Down", "D: Right" }
			, movingTex{std::make_unique<rcgf::Texture>(renderer, "img/dice.png")}
			, pos{0.f,0.f}
		{
			for (size_t i = 0; i < NUM_OPTIONS; ++i)
			{
				texArr[i] = std::make_unique<rcgf::Texture>(
					renderer,
					font, 
					optionsStrings[i].c_str()
				);
			}

			animation = std::make_unique<rcgf::Animation>(std::make_unique<rcgf::Texture>(renderer, "img/chicken_2.png"), SPRITE_W, SPRITE_H, 4, 4);
		}
	};

	GameSceneUserInput::GameSceneUserInput(SDL_Renderer* renderer, TTF_Font* font)
		: m_impl{ std::make_unique<Pimpl>(renderer, font) }, m_renderer{renderer}, m_font{font}
	{}

	void GameSceneUserInput::update()
	{
		++m_impl->animCounter;
		if (m_impl->animCounter > 5)
		{
			m_impl->animCounter = 0;
			++m_impl->animIdx;
			if (m_impl->animIdx > 7)
			{
				m_impl->animIdx = 0;
			}
		}
		m_impl->scaler.update();
		m_impl->pos += m_impl->velocity;
	}

	void GameSceneUserInput::render(SDL_Renderer* renderer)
	{
		// 4100
		renderer;

		// get mouse position
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			std::stringstream ss;
			ss << "x:" << x << " y:" << y;
			global::Global::renderMonospaceText(ss.str(), x, y);
		}

		for (int i = 0; i < NUM_OPTIONS; ++i)
		{
			m_impl->texArr[i]->render(30, 30 + 30 * i);
		}
		const double angle = m_impl->scaler.getScale();

		m_impl->movingTex->render(
			static_cast<int>(m_impl->pos.x),
			static_cast<int>(m_impl->pos.y),
			NULL,
			1.f,
			angle
		);

		m_impl->animation->render(m_impl->animIdx, 600, 50);
	}

	void GameSceneUserInput::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
		case SDLK_ESCAPE:
			printf("switching to game state main from user input\n"); 
			pushPendingState(std::make_unique<GameSceneMain>(m_renderer, m_font));
			break;
		case SDLK_w:
			m_impl->velocity.y = -VELOCITY;
			break;
		case SDLK_a:
			m_impl->velocity.x = -VELOCITY;
			break;
		case SDLK_s:
			m_impl->velocity.y = VELOCITY;
			break;
		case SDLK_d:
			m_impl->velocity.x = VELOCITY;
			break;
		}
	}

	void GameSceneUserInput::keyUp(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_w:
		case SDLK_s:
			m_impl->velocity.y = 0;
			break;
		case SDLK_a:
		case SDLK_d:
			m_impl->velocity.x = 0;
			break;
		}
	}
}
