#pragma once

#include <string>
#include <memory>
#include "GameInclude.h"

namespace game
{
	////Texture wrapper class
class Texture
{
public:
	//Initializes variables
	Texture() = delete;

	Texture(SDL_Renderer* renderer, const char* pngPath);
	Texture(SDL_Renderer* renderer, TTF_Font* font, const char* text);

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
	SDL_Renderer* m_renderer;
	
	// TODO: move this to its own class.
	//TTF_Font* m_font = NULL;

	//Image dimensions
	int m_width;
	int m_height;
};
}