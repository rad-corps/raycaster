
#include "Globals.h"
#include "Texture.h"
#include "game/scenes/GameSceneMain.h"
#include <cassert>
#include <cstdio>
#include <array>
#include <string>
#include <chrono>
#include <Windows.h>
#include <WinUser.h>
#include "config/DesignerConstants.h"


namespace
{
	constexpr double FIXED_UPDATE = 1000 / 60.0;
}

int main(int argc, char* args[])
{
	// set console position to top left
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// satisfy compiler warning for not referencing parameters
	for (int i = 0; i < argc; ++i)
	{
		printf("command line args %u %s\n", i, args[i]);
	}

	// initialise SDL and other systems such as font and PNG loading
	global::SDL_Global sdlGlobal = global::Global::init();
	SDL_Renderer* renderer = sdlGlobal.renderer;
	TTF_Font* font = sdlGlobal.font;

	printf("SDL Initialisation complete\n");
	
	std::unique_ptr<rcgf::IGameScene> gameState = std::make_unique<game::GameSceneMain>(renderer, font);

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	// used to calculate call to fixedUpdate 
	double deltaTimeMs = 0.f;

	std::chrono::steady_clock::time_point time1 = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point time2 = std::chrono::high_resolution_clock::now();

	SimplePerfCounter fpsPerfCounter;

	// TODO: this should be the same variable as fpsPerfCounter
	// but there is currently no method to stop and get both the 
	// ms and fps in one go
	SimplePerfCounter deltaTimeCalculator; 


	std::string fps;
	double deltatime = 0.0;

	//While application is running
	while (!quit)
	{
		fpsPerfCounter.Start();
		deltaTimeCalculator.Start();

		time1 = std::chrono::high_resolution_clock::now();
		// handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				gameState->keyDown(e.key.keysym.sym);
				break;
			case SDL_KEYUP:
				gameState->keyUp(e.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				gameState->mouseDown((int)e.button.button, (int)e.button.x, (int)e.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				gameState->mouseUp((int)e.button.button, (int)e.button.x, (int)e.button.y);
				break;
			case SDL_MOUSEMOTION:
				gameState->mouseMove(e.motion.xrel, e.motion.yrel);
				break;
			case SDL_JOYAXISMOTION:
				//Motion on controller 0
				if (abs(e.jaxis.value) > design::THUMBSTICK_DEADZONE)
				{
					gameState->gamepadEvent(e.jaxis.which, (int)e.jaxis.axis, e.jaxis.value);
				}
				else
				{
					gameState->gamepadEvent(e.jaxis.which, (int)e.jaxis.axis, 0);
				}
			}
		}

		// clear screen before drawing
		SDL_SetRenderDrawColor(renderer, 60, 60, 72, 0xFF);
		SDL_RenderClear(renderer);

		gameState->update();
		if (gameState->hasPendingState())
		{
			gameState = gameState->popPendingState();
		}
		else
		{
			// all drawing should happen here
			gameState->render(deltatime);
		}

		// show fps
		global::Global::renderMonospaceText("fps:   " + fps, SCREEN_WIDTH - 160, 0);

		// swap the framebuffer
		SDL_RenderPresent(renderer);
		time2 = std::chrono::high_resolution_clock::now();
		deltaTimeMs += std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() / 1000.0;

		// TODO: there is a bug where this update will attempt to catch up if we have not been hitting our target FPS
		// to reproduce, turn on the slow floor rendering, then look directly at a wall while rotating. 
		if (deltaTimeMs > FIXED_UPDATE)
		{
			deltaTimeMs -= FIXED_UPDATE;
			gameState->fixedUpdate();
		}

		fps = fpsPerfCounter.StopFPS();
		deltatime = deltaTimeCalculator.StopMs();
	}

	return 0;
}
