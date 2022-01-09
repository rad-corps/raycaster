#pragma once

#pragma warning( push )

#pragma warning( disable: 26812 26819 4201 )

#include "SDL.h"
#include "SDL_log.h"
#include "SDL_mouse.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "glm/vec2.hpp"

#pragma warning( pop )

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

namespace global
{
	class Global
	{
	public: 
		TTF_Font* getFont();
		SDL_Renderer* getRenderer();
		void setRenderer(SDL_Renderer*);
		void setFont(TTF_Font*);
	};

	extern Global instance;

}