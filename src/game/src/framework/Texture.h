#pragma once

#include <string>
#include <memory>
#include "Globals.h"

namespace rcgf
{
	class Texture
	{
	public:
		// don't allow default construction
		// we don't want to allow a Texture to be in an invalid state. 
		Texture() = delete;

		// remove copy construction/assignment
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		// enforce default move construction and assignment
		Texture(Texture&&) = default;
		Texture& operator=(Texture&&) = default;

		// create a texture from a .PNG file
		Texture(const char* pngPath);

		// create a texture from a string with a given font
		Texture(TTF_Font* font, const char* text);

		//Set color modulation
		void setColor(Uint8 red, Uint8 green, Uint8 blue);

		//Set blending
		void setBlendMode(SDL_BlendMode blending);

		//Set alpha modulation
		void setAlpha(Uint8 alpha);

		//Renders texture at given point
		void render(int x, int y, SDL_Rect* clip = NULL, float scale = 1.0f, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE, bool drawFromCenter = false);

		// get image dimensions
		int getWidth();
		int getHeight();

	private:

		// The representation of a hardware texture
		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
	
		//Image dimensions
		int m_width;
		int m_height;
	};
}