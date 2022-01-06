#pragma once

#include <string>
#include <array>
#include <memory>
#include "IGameState.h"
#include "GameTexture.h"
#include "GameInclude.h"

namespace game
{
	class GameStateMoving : public IGameState
	{
	public:
		GameStateMoving(SDL_Renderer* renderer, TTF_Font* font);
		std::unique_ptr<IGameState> update() override;
		void render() override;
		void keyDown(SDL_Keycode) override;
		void keyUp(SDL_Keycode) override;

	private:
		struct Pimpl;
		std::unique_ptr<Pimpl> m_impl;
	};
}
