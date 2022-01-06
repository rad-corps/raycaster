
#include "GameInclude.h"
#include "GameTexture.h"
#include "GameStateMain.h"
#include <cassert>
#include <cstdio>
#include <array>
#include <string>

namespace
{
	struct SDL_Structure
	{
		SDL_Renderer* renderer;
		SDL_Window* window;
	};

	TTF_Font* loadFont(const char* font, int fontSz)
	{
		TTF_Font* ret = TTF_OpenFont(font, fontSz);
		assert(ret != NULL);
		return ret;
	}

	SDL_Structure init()
	{
		SDL_Structure ret;
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

		ret.window = SDL_CreateWindow(
			"SDL Game",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
		);

		ret.renderer = SDL_CreateRenderer(
			ret.window,
			-1, // index of the rendering driver to init. -1 for first available 
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		);

		return ret;
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
	SDL_Structure sdl = init();

	printf("SDL Initialisation complete");

	// load fonts and textures
	TTF_Font* font = loadFont("font/PTC55F.ttf", 28);
	
	std::unique_ptr<game::IGameState> gameState(new game::GameStateMain(sdl.renderer, font));

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
		}

		// clear screen
		SDL_SetRenderDrawColor(sdl.renderer, 0x00, 0x00, 0x20, 0xFF);
		SDL_RenderClear(sdl.renderer);

		std::unique_ptr<game::IGameState> newState = gameState->update();
		if (newState)
		{
			gameState = std::move(newState);
		}

		//Update screen
		SDL_RenderPresent(sdl.renderer);
	}

	return 0;
}
