#pragma once

#include <memory> // std::unique_ptr
#include "../GameInclude.h"

namespace game
{
	class IGameScene
	{
	public:
		virtual ~IGameScene() = default;

		// return nullptr on no change. 
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void keyDown(SDL_Keycode) = 0;
		virtual void keyUp(SDL_Keycode) = 0;

		void pushPendingState(std::unique_ptr<IGameScene>);
		bool hasPendingState();
		std::unique_ptr<IGameScene> popPendingState();

	private:
		std::unique_ptr<IGameScene> m_nextState;

	};
}