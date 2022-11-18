#pragma once

#include <string>
#include <array>
#include <memory>
#include "IGameScene.h"
#include "Texture.h"
#include "Globals.h"

namespace game
{
	class GameSceneUserInput : public rcgf::IGameScene
	{
	public:
		GameSceneUserInput(SDL_Renderer* renderer, TTF_Font* font);
		GameSceneUserInput() = delete;
		void update() override;
		void fixedUpdate() override {};
		void render(SDL_Renderer* renderer) override;
		void keyDown(SDL_Keycode) override;
		void keyUp(SDL_Keycode) override;
		void mouseDown(int button, int x, int y) override { button; x; y; };

	private:
		struct Pimpl;
		std::unique_ptr<Pimpl> m_impl;
		SDL_Renderer* m_renderer;
		TTF_Font* m_font;
	};
}
