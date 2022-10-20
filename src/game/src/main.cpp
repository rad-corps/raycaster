
#include "Globals.h"
#include "Texture.h"
#include "game/scenes/GameSceneMain.h"
#include <cassert>
#include <cstdio>
#include <array>
#include <string>
#include <chrono>


namespace
{
	constexpr double FIXED_UPDATE = 1000 / 60.0;

	void init()
	{
		global::instance.init();
	}
}

int main(int argc, char* args[])
{
	// satisfy compiler warning for not referencing parameters
	for (int i = 0; i < argc; ++i)
	{
		printf("command line args %u %s\n", i, args[i]);
	}

	// initialise SDL and other systems such as font and PNG loading
	init();

	printf("SDL Initialisation complete\n");

	
	
	std::unique_ptr<rcgf::IGameScene> gameState = std::make_unique<game::GameSceneMain>();

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	// used to calculate call to fixedUpdate 
	double deltaTimeMs = 0.f;

	std::chrono::steady_clock::time_point time1 = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point time2 = std::chrono::high_resolution_clock::now();

	SimplePerfCounter renderPerfCounter;
	SimplePerfCounter fpsPerfCounter;
	std::string renderTime;
	std::string fps;

	//While application is running
	while (!quit)
	{
		fpsPerfCounter.Start();

		time1 = std::chrono::high_resolution_clock::now();
		// handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// user requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				gameState->keyDown(e.key.keysym.sym);
			}
			else if (e.type == SDL_KEYUP)
			{
				gameState->keyUp(e.key.keysym.sym);
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				gameState->mouseDown((int)e.button.button, (int)e.button.x, (int)e.button.y);
			}
		}

		// clear screen
		SDL_SetRenderDrawColor(global::instance.getRenderer(), 0x00, 0x00, 0x20, 0xFF);
		SDL_RenderClear(global::instance.getRenderer());

		gameState->sendData(renderTime, fps);
		gameState->update();
		if (gameState->hasPendingState())
		{
			gameState = gameState->popPendingState();
		}
		else
		{
			gameState->render();
		}

		//Update screen
		renderPerfCounter.Start();
		SDL_RenderPresent(global::instance.getRenderer());
		renderTime = renderPerfCounter.Stop();

		time2 = std::chrono::high_resolution_clock::now();
		deltaTimeMs += std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() / 1000.0;

		if (deltaTimeMs > FIXED_UPDATE)
		{
			deltaTimeMs -= FIXED_UPDATE;
			gameState->fixedUpdate();
		}

		fps = fpsPerfCounter.StopFPS();
	}

	return 0;
}
