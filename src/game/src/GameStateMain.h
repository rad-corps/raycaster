#pragma once

#include <string>
#include <array>
#include <memory>
#include "IGameState.h"
#include "GameTexture.h"
#include "GameInclude.h"

namespace game
{
	class GameStateMain : public IGameState
	{
	public:
		GameStateMain(SDL_Renderer* renderer, TTF_Font* font);
		std::unique_ptr<IGameState> update() override;
		void render() override {};
		void keyDown(SDL_Keycode) override;
		void keyUp(SDL_Keycode) override;

	private:
		struct Pimpl;
		std::unique_ptr<Pimpl> m_impl;

		const std::array<std::string, 4> m_optionsStrings;
		std::array<std::unique_ptr<game::Texture>, 4> m_texArr;
		std::unique_ptr<game::Texture> m_bgTex;
	};
}
