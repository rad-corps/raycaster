#pragma once

#include <string>
#include <array>
#include <memory>
#include "IGameScene.h"
#include "Texture.h"
#include "Globals.h"

namespace game
{
	class GameSceneMain : public rcgf::IGameScene
	{
	public:
		GameSceneMain(SDL_Renderer* renderer, TTF_Font* font);
		GameSceneMain() = delete;
		void update() override;
		void fixedUpdate() override {};
		void render(double deltatime) override;
		void keyDown(SDL_Keycode) override;
		void keyUp(SDL_Keycode) override;

	private:

		void keyDownCapturingText(SDL_Keycode);
		void keyDownImpl(SDL_Keycode);
		struct Pimpl;
		std::unique_ptr<Pimpl> m_impl;
		SDL_Renderer* m_renderer;
		TTF_Font* m_font;
		std::string m_mapName = "./rooms/out/map-01";
		std::string m_mapTempName = "";
		bool m_capturingMapName = false;
	};
}
