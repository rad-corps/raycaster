#include "Globals.h"
#include <cstdio>
#include <cassert>
#include "SpriteSheet.h"
#include <chrono>

namespace
{
	std::unique_ptr<rcgf::SpriteSheet> monoText;

	TTF_Font* loadFont(const char* font_, int fontSz)
	{
		TTF_Font* ret = TTF_OpenFont(font_, fontSz);
		assert(ret != NULL);
		return ret;
	}

	
	// deltatime calculation.. hacky :(
	std::chrono::steady_clock::time_point prevTime;
	std::chrono::steady_clock::time_point currTime;
	double elapsed_time_ms = 0.0;

	SDL_Joystick* gGameController = NULL;
}

void SDL_SetRenderDrawColor(SDL_Renderer* renderer, Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

namespace global
{
	//Global instance;
	SDL_Global Global::init()
	{
		SDL_Global ret{};
		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
		{
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			assert(false);
		}

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
		{
			printf("Warning: Nearest pixel filtering not enabled!");
			assert(false);
		}

		//Initialize PNG loading
		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			assert(false);
		}

		//Initialize SDL_ttf
		if (TTF_Init() == -1)
		{
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			assert(false);
		}

		ret.window = SDL_CreateWindow(
			"SDL Game",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
		);

		if (!ret.window)
		{
			assert(false);
		}

		//Check for joysticks
		if (SDL_NumJoysticks() > 0)
		{
			//Load joystick
			gGameController = SDL_JoystickOpen(0);
			if (gGameController == NULL)
			{
				printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
			}
		}

		
		ret.renderer = SDL_CreateRenderer(
			ret.window,
			-1, // index of the rendering driver to init. -1 for first available 
			//SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
			SDL_RENDERER_ACCELERATED
		);

		if (!ret.renderer)
		{
			assert(false);
		}
		

		// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);       // changes video mode
		// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); // fake full screen
		

		SDL_SetRenderDrawBlendMode(ret.renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);

		// load fonts and textures
		ret.font = loadFont("font/PTC55F.ttf", 28);


		// works but its too big
		monoText = std::make_unique<rcgf::SpriteSheet>(
			std::make_unique<rcgf::Texture>(ret.renderer, "img/mono_font.png"), 
			20, // sprite width
			20, // sprite height
			8,  // rows
			15  // cols
		);

		// SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_TRUE);

		return ret;
	}

	void Global::renderMonospaceText(const std::string& inp, int x, int y)
	{
		for (const char& c : inp)
		{
			monoText->render(static_cast<int>(c - 32), x, y);
			x += 12;
		}
	}
}