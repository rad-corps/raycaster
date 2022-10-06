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
		GameSceneRaycaster();
		void update() override;
		void fixedUpdate() override;
		void render() override;
		void keyDown(SDL_Keycode) override;
		void keyUp(SDL_Keycode) override;
		void mouseDown(int button, int x, int y) override;

	private:
		struct Pimpl;
		std::unique_ptr<Pimpl> m_impl;
	};
}
