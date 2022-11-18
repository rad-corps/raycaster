#pragma once

#include <string>
#include <array>
#include <memory>
#include "IGameScene.h"
#include "Texture.h"
#include "Globals.h"

namespace game
{
	class GameSceneTransform : public rcgf::IGameScene
	{
	public:
		GameSceneTransform(SDL_Renderer* renderer, TTF_Font* font);
		GameSceneTransform() = delete;
		void update() override;
		void fixedUpdate() override {};
		void render(SDL_Renderer* renderer) override;
		void keyDown(SDL_Keycode) override;
		void keyUp(SDL_Keycode) override;
		void mouseDown(int button, int x, int y) override { button; x; y; };

	private:
		SDL_Renderer* m_renderer;
		TTF_Font* m_font;
		struct Pimpl;
		std::unique_ptr<Pimpl> m_impl;
	};
}
