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

#include <chrono>
#include <sstream>
#include <iomanip>

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

struct Color
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};

struct Line
{
	SDL_Point start;
	SDL_Point end;
};

struct SimplePerfCounter
{
	void Start()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	double StopMs()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0;
	}
	std::string StopFPS()
	{
		double fps = 1000.0 / StopMs();
		std::stringstream ss;
		ss << std::fixed << std::setprecision(1) << fps;
		return ss.str();
	}
	std::string Stop()
	{
		double result = StopMs();
		std::stringstream ss;
		ss << std::fixed << std::setprecision(1) << result;
		return ss.str();
	}

	std::chrono::steady_clock::time_point start;
};