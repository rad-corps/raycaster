#pragma once

#pragma warning( push )

#pragma warning( disable: 26812 26819 4201 )

#include "SDL.h"
#include "SDL_log.h"
#include "SDL_mouse.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "glm/vec2.hpp"
#include "glm/gtx/vector_angle.hpp"
#include <string>

#pragma warning( pop )

constexpr int SCREEN_WIDTH = 1920;
constexpr int CENTER_X = SCREEN_WIDTH / 2;
constexpr int SCREEN_HEIGHT = 1080;
constexpr int CENTER_Y = SCREEN_HEIGHT / 2;
constexpr float PI = 3.14159265359f;

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