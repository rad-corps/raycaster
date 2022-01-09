
#include "Globals.h"
#include "Texture.h"
#include "game/scenes/GameSceneMain.h"
#include <cassert>
#include <cstdio>
#include <array>
#include <string>

namespace
{
	TTF_Font* loadFont(const char* font, int fontSz)
	{
		TTF_Font* ret = TTF_OpenFont(font, fontSz);
		assert(ret != NULL);
		return ret;
	}

	void init()
	{
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		}

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
		{
			printf("Warning: Nearest pixel filtering not enabled!");
		}

		//Initialize PNG loading
		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		}

		//Initialize SDL_ttf
		if (TTF_Init() == -1)
		{
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		}

		auto window = SDL_CreateWindow(
			"SDL Game",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
		);

		global::instance.setRenderer(
			SDL_CreateRenderer(
				window,
				-1, // index of the rendering driver to init. -1 for first available 
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
			)
		);
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

	// load fonts and textures
	global::instance.setFont(loadFont("font/PTC55F.ttf", 28));
	
	std::unique_ptr<rcgf::IGameScene> gameState = std::make_unique<game::GameSceneMain>();

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit)
	{
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
		}

		// clear screen
		SDL_SetRenderDrawColor(global::instance.getRenderer(), 0x00, 0x00, 0x20, 0xFF);
		SDL_RenderClear(global::instance.getRenderer());

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
		SDL_RenderPresent(global::instance.getRenderer());
	}

	return 0;
}
