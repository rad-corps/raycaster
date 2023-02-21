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
		GameSceneRaycaster(SDL_Renderer* renderer, TTF_Font* font, const std::string& mapFile);
		GameSceneRaycaster() = delete;
		void update() override;
		void fixedUpdate() override;
		void render(double deltatime) override;
		void keyDown(SDL_Keycode) override;
		void keyUp(SDL_Keycode) override;
		void gamepadEvent(int gamepadNumber, int buttonOrAxis, int value) override;

	private:
		struct Pimpl;
		std::unique_ptr<Pimpl> m_impl;
		SDL_Renderer* m_renderer;
		TTF_Font* m_font;
	};
}
