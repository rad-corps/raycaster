#include "GameSceneTransform.h"
#include "GameSceneMain.h"
#include "RCGFMath.h"

namespace
{
	constexpr int NUM_OPTIONS = 6;

	class Translator
	{
	private:
		enum class DirectionH
		{
			RIGHT,
			LEFT
		} directionH;

		enum class DirectionV
		{
			UP,
			DOWN
		} directionV;

		const float VELOCITY = 4.f;
		
		math::Vec2f pos;
		bool active = false;

	public:
		Translator(math::Vec2f startingPos)
			: directionH{DirectionH::RIGHT}
			, directionV{DirectionV::DOWN}
			, pos{startingPos}
		{}
		
		const math::Vec2f& getPos()
		{
			return pos;
		}
		
		void toggleActive()
		{
			active = !active;
		}

		void update()
		{
			if (!active) return;
			
			directionH == DirectionH::RIGHT ? pos.x += VELOCITY : pos.x -= VELOCITY;
			if ( pos.x > SCREEN_WIDTH ) directionH = DirectionH::LEFT;
			else if ( pos.x < 0 ) directionH = DirectionH::RIGHT;

			directionV == DirectionV::UP ? pos.y -= VELOCITY : pos.y += VELOCITY;
			if ( pos.y > SCREEN_HEIGHT ) directionV = DirectionV::UP;
			else if ( pos.y < 0 ) directionV = DirectionV::DOWN;
		}
	};

	class Rotator
	{
	private:
		double m_rot = 0.0;
		bool active = false;
	
	public:
		void toggleActive()
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

		void toggleActive()
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
	struct GameSceneTransform::Pimpl
	{
		std::array<std::string, NUM_OPTIONS> optionsStrings;
		std::array<std::unique_ptr<rcgf::Texture>, NUM_OPTIONS> texArr;
		std::unique_ptr<rcgf::Texture> movingTex;
		Scaler scaler;
		Rotator rotator;
		Translator translator;

		bool flipH = false;
		bool flipV = false;

		Pimpl(SDL_Renderer* renderer, TTF_Font* font) 
			: optionsStrings{"0: Back", "1: Scaling", "2: Rotation", "3: Translate", "4: Flip Horizontal", "5: Flip Vertical"}
			, movingTex{std::make_unique<rcgf::Texture>(renderer, "img/dice.png")}
			, translator(
				math::Vec2f(
					static_cast<float>(SCREEN_WIDTH / 2),
					static_cast<float>(SCREEN_HEIGHT / 2)
				)
			)
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

	GameSceneTransform::GameSceneTransform(SDL_Renderer* renderer, TTF_Font* font)
		: m_impl{ std::make_unique<Pimpl>(renderer, font) }
		, m_renderer{renderer}
		, m_font{font}
	{}

	void GameSceneTransform::update()
	{
		m_impl->scaler.update();
		m_impl->rotator.update();
		m_impl->translator.update();
	}

	void GameSceneTransform::render(SDL_Renderer* renderer)
	{
		// 4100
		renderer;

		for (int i = 0; i < NUM_OPTIONS; ++i)
		{
			m_impl->texArr[i]->render(30, 30 + 30 * i);
		}
		const float scale = m_impl->scaler.getScale();
		const double angle = m_impl->rotator.getRotation();
		const math::Vec2f& pos = m_impl->translator.getPos();

		
		int flip = SDL_FLIP_NONE;
		if ( m_impl->flipH ) flip |= SDL_FLIP_HORIZONTAL;
		if ( m_impl->flipV ) flip |= SDL_FLIP_VERTICAL;

		m_impl->movingTex->render(
			static_cast<int>(pos.x),
			static_cast<int>(pos.y),
			NULL,
			scale,
			angle,
			static_cast<SDL_RendererFlip>(flip),
			true
		);
	}

	void GameSceneTransform::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
			printf("switching to game state main\n"); 
			pushPendingState(std::make_unique<GameSceneMain>(m_renderer, m_font));
			break;
		case SDLK_KP_1:
		case SDLK_1:
			m_impl->scaler.toggleActive();
			break;
		case SDLK_KP_2:
		case SDLK_2:
			m_impl->rotator.toggleActive();
			break;
		case SDLK_KP_3:
		case SDLK_3:
			m_impl->translator.toggleActive();
			break;
		case SDLK_KP_4:
		case SDLK_4:
			m_impl->flipH = !m_impl->flipH;
			break;
		case SDLK_KP_5:
		case SDLK_5:
			m_impl->flipV = !m_impl->flipV;
			break;

		}

	}

	void GameSceneTransform::keyUp(SDL_Keycode)
	{

	}
}
