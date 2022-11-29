#pragma once

#include <memory> // std::unique_ptr
#include "Globals.h"
#include <map>
#include <iostream>

namespace rcgf
{
	class IGameScene
	{
	public:
		virtual ~IGameScene() = default;

		// return nullptr on no change. 
		virtual void fixedUpdate() = 0;
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void keyDown(SDL_Keycode) = 0;
		virtual void keyUp(SDL_Keycode) = 0;
		virtual void mouseDown(int button, int x, int y) 
		{
			std::cout << "mouseDown not implemented: " << button << x << y << std::endl;
		}

		void pushPendingState(std::unique_ptr<IGameScene>);
		bool hasPendingState();
		std::unique_ptr<IGameScene> popPendingState();

	private:
		std::unique_ptr<IGameScene> m_nextState;

	};
}