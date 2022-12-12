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
		Texture(SDL_Renderer* renderer, const char* pngPath);

		// create a texture from a string with a given font
		Texture(SDL_Renderer* renderer, TTF_Font* font, const char* text);

		//Set color modulation
		void setColor(Uint8 red, Uint8 green, Uint8 blue);

		//Set blending
		void setBlendMode(SDL_BlendMode blending);

		//Set alpha modulation
		void setAlpha(Uint8 alpha);

		// TODO: can we comment this out so no clients have access to the raw pointer?
		SDL_Texture* get();

		// TODO: better names for these two render functions
		void render(int x, int y, SDL_Rect* clip = NULL, float scale = 1.0f, double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE, bool drawFromCenter = false) const;
		void render2(const SDL_Rect* textureClip, const SDL_Rect* outputClip) const;
		

		Color getPixelColor(int col, int row);

		// get image dimensions
		int getWidth();
		int getHeight();

		void printDebugInfo();

	private:

		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
		std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> m_surface;
		SDL_Renderer* m_renderer;
		std::string m_name;
	
		//Image dimensions
		int m_width;
		int m_height;
	};
}