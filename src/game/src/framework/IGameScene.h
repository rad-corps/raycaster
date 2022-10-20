#pragma once

#include <memory> // std::unique_ptr
#include "Globals.h"
#include <map>


namespace rcgf
{
	class IGameScene
	{
	public:
		virtual ~IGameScene() = default;

		// return nullptr on no change. 
		virtual void fixedUpdate() = 0;
		virtual void sendData(const std::string& renderTime, const std::string& fps) { renderTime; fps; };
		virtual void update() = 0;
		virtual void render() = 0;
		virtual void keyDown(SDL_Keycode) = 0;
		virtual void keyUp(SDL_Keycode) = 0;
		virtual void mouseDown(int button, int x, int y) = 0;

		void pushPendingState(std::unique_ptr<IGameScene>);
		bool hasPendingState();
		std::unique_ptr<IGameScene> popPendingState();

	private:
		std::unique_ptr<IGameScene> m_nextState;

	};
}