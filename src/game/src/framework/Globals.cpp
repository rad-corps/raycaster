#include "Globals.h"
#include <cstdio>
#include "Animation.h"

namespace
{
	TTF_Font* font;
	SDL_Renderer* renderer;
	std::unique_ptr<rcgf::Animation> monoText;

	TTF_Font* loadFont(const char* font_, int fontSz)
	{
		TTF_Font* ret = TTF_OpenFont(font_, fontSz);
		assert(ret != NULL);
		return ret;
	}

	void setRenderer(SDL_Renderer* renderer_)
	{
		renderer = renderer_;
	}
	void setFont(TTF_Font* font_)
	{
		font = font_;
	}
}

namespace global
{
	Global instance;
	void Global::init()
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

		setRenderer(
			SDL_CreateRenderer(
				window,
				-1, // index of the rendering driver to init. -1 for first available 
				//SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
				SDL_RENDERER_ACCELERATED
			)
		);

		// load fonts and textures
		setFont(loadFont("font/PTC55F.ttf", 28));


		// works but its too big
		monoText = std::make_unique<rcgf::Animation>(
			std::make_unique<rcgf::Texture>("img/mono_font.png"), 
			20, // sprite width
			20, // sprite height
			8,  // rows
			15  // cols
		);
	}

	void Global::renderMonospaceText(const std::string& inp, int x, int y)
	{
		for (const char& c : inp)
		{
			monoText->render(static_cast<int>(c - 32), x, y);
			x += 12;
		}
	}

	TTF_Font* Global::getFont()
	{
		return font;
	}

	SDL_Renderer* Global::getRenderer()
	{
		return renderer;
	}
}