#pragma once

#include <memory> // std::unique_ptr
#include "../GameInclude.h"

namespace game
{
	class IGameState
	{
	public:
		virtual ~IGameState() = default;

		// return nullptr on no change. 
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void keyDown(SDL_Keycode) = 0;
		virtual void keyUp(SDL_Keycode) = 0;

		void pushPendingState(std::unique_ptr<IGameState>);
		bool hasPendingState();
		std::unique_ptr<IGameState> popPendingState();

	private:
		std::unique_ptr<IGameState> m_nextState;

	};
}