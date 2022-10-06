#pragma once

#include <string>
#include <array>
#include <memory>
#include "IGameScene.h"
#include "Texture.h"
#include "Globals.h"

namespace game
{
	class GameScenePolygon : public rcgf::IGameScene
	{
	public:
		GameScenePolygon();
		void update() override;
		void fixedUpdate() override {};
		void render() override;
		void keyDown(SDL_Keycode) override;
		void keyUp(SDL_Keycode) override;
		void mouseDown(int button, int x, int y) override { button; x; y; };

	private:
		struct Pimpl;
		std::unique_ptr<Pimpl> m_impl;
	};
}
