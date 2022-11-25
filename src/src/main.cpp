
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
}

int main(int argc, char* args[])
{
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

	SimplePerfCounter renderPerfCounter;
	SimplePerfCounter fpsPerfCounter;
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
		SDL_SetRenderDrawColor(renderer, 60, 60, 72, 0xFF);
		SDL_RenderClear(renderer);

		gameState->update();
		if (gameState->hasPendingState())
		{
			gameState = gameState->popPendingState();
		}
		else
		{
			gameState->render(renderer);
		}

		//Update screen
		renderPerfCounter.Start();

		// show fps
		global::Global::renderMonospaceText("fps:   " + fps, SCREEN_WIDTH - 160, 0);


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
	}

	return 0;
}
