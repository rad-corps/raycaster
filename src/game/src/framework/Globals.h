#pragma once

#pragma warning( push )

#pragma warning( disable: 26812 26819 4201 )

#include "SDL.h"
#include "SDL_log.h"
#include "SDL_mouse.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "glm/vec2.hpp"
#include <string>

#pragma warning( pop )

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

namespace global
{
	class Global
	{
	public: 
		void init();
		TTF_Font* getFont();
		SDL_Renderer* getRenderer();
		void renderMonospaceText(const std::string& inp, int x, int y);
	};

	extern Global instance;

}