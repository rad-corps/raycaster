#pragma once

#include <memory>
#include "IGameScene.h"
#include "Texture.h"
#include "Globals.h"

namespace game
{
	class GameSceneRaycaster: public rcgf::IGameScene
	{
	public:
		GameSceneRaycaster(SDL_Renderer* renderer, TTF_Font* font);
		GameSceneRaycaster() = delete;
		void update() override;
		void fixedUpdate() override;
		void render(SDL_Renderer* renderer) override;
		void keyDown(SDL_Keycode) override;
		void keyUp(SDL_Keycode) override;
		void mouseDown(int button, int x, int y) override;
		void sendData(const std::string& data, const std::string& fps) override;

	private:
		struct Pimpl;
		std::unique_ptr<Pimpl> m_impl;
		SDL_Renderer* m_renderer;
		TTF_Font* m_font;
	};
}
