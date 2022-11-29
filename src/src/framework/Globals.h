#pragma once

// warning C4514: 'SimplePerfCounter::Start': unreferenced inline function has been removed
#pragma warning( disable: 4515 )

#include <SDL.h>
#include <SDL_log.h>
#include <SDL_mouse.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>

#include <chrono>
#include <sstream>
#include <iomanip>

constexpr int SCREEN_WIDTH = 1920;
constexpr int CENTER_X = SCREEN_WIDTH / 2;
constexpr int SCREEN_HEIGHT = 1080;
constexpr int CENTER_Y = SCREEN_HEIGHT / 2;
//constexpr float PI = 3.14159265359f;

namespace global
{
	struct SDL_Global
	{
		SDL_Renderer* renderer;
		SDL_Window* window;
		TTF_Font* font;
	};
	class Global
	{
	public: 
		static SDL_Global init();
		//TTF_Font* getFont();
		//SDL_Renderer* getRenderer();
		static void renderMonospaceText(const std::string& inp, int x, int y);
	};

	// extern Global instance;

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
		double microseconds = (double)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count());
		return microseconds / 1000.0;
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