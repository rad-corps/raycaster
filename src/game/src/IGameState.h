#pragma once

#include <memory> // std::unique_ptr
#include "GameInclude.h"

namespace game
{
	class IGameState
	{
	public:
		virtual ~IGameState() = default;

		// return nullptr on no change. 
		virtual std::unique_ptr<IGameState> update() = 0;
		virtual void render() = 0;
		virtual void keyDown(SDL_Keycode) = 0;
		virtual void keyUp(SDL_Keycode) = 0;

	};
}