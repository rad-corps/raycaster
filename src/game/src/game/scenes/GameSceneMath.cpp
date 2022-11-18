#include "GameSceneMath.h"
#include "GameSceneMain.h"
#include <iostream>
#include <sstream>

namespace game
{
	struct GameSceneMath::Pimpl
	{
		Pimpl()
		{
		}
	};

	GameSceneMath::GameSceneMath(SDL_Renderer* renderer, TTF_Font* font)
		: m_impl{std::make_unique<Pimpl>()}
		, m_renderer{renderer}
		, m_font{font}
	{}

	void GameSceneMath::update()
	{
		
	}

	void GameSceneMath::render(SDL_Renderer* renderer)
	{
		// 4100
		renderer;

		SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		// render coordinate system

		// draw x axis
		SDL_RenderDrawLine(m_renderer,
			0,
			CENTER_Y,
			SCREEN_WIDTH,
			CENTER_Y
		);

		// draw y axis
		SDL_RenderDrawLine(m_renderer,
			CENTER_X,
			0,
			CENTER_X,
			SCREEN_HEIGHT
		);

		// draw a line from center to mouse
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		// draw vector
		SDL_RenderDrawLine(m_renderer,
			CENTER_X,
			CENTER_Y,
			mouseX,
			mouseY
		);

		// treat the center of the screen as 0, 0
		std::stringstream vecCoords;
		glm::vec2 mouseVec{ static_cast<float>(mouseX - CENTER_X), static_cast<float>(mouseY - CENTER_Y) };
		glm::vec2 referenceVec{ 100.f, 0.f };
		vecCoords << "x" << mouseVec.x << "y" << mouseVec.y;
		float angle = glm::angle(glm::normalize(mouseVec), glm::normalize(referenceVec));
		float dot = glm::dot(mouseVec, referenceVec);
		global::Global::renderMonospaceText(vecCoords.str(), mouseX + 20, mouseY);
		global::Global::renderMonospaceText("angle:" + std::to_string(angle), CENTER_X, CENTER_Y);
		global::Global::renderMonospaceText("dot  :" + std::to_string(dot), CENTER_X, CENTER_Y + 20);
	}

	void GameSceneMath::keyDown(SDL_Keycode keycode)
	{
		switch (keycode)
		{
		case SDLK_0:
		case SDLK_KP_0:
		case SDLK_ESCAPE:
			pushPendingState(std::make_unique<GameSceneMain>(m_renderer, m_font));
			break;
		}
	}
	
	void GameSceneMath::keyUp(SDL_Keycode)
	{

	}
}
