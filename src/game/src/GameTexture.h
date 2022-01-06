#pragma once

#include <string>
#include <memory>
#include "GameInclude.h"

namespace game
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
		Texture(Texture&&) = delete;
		Texture& operator=(Texture&&) = delete;

		// create a texture from a .PNG file
		Texture(SDL_Renderer* renderer, const char* pngPath);

		// create a texture from a string with a given font
		Texture(SDL_Renderer* renderer, TTF_Font* font, const char* text);

		// constructor overloads call init overload
		// direct init call required when using default construction
		bool init(SDL_Renderer* renderer, const char* pngPath);
		bool init(SDL_Renderer* renderer, TTF_Font* font, const char* text);

		//Set color modulation
		void setColor(Uint8 red, Uint8 green, Uint8 blue);

		//Set blending
		void setBlendMode(SDL_BlendMode blending);

		//Set alpha modulation
		void setAlpha(Uint8 alpha);

		//Renders texture at given point
		void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		// get image dimensions
		int getWidth();
		int getHeight();

	private:
		bool initImpl(const char* pngPath);
		bool initImpl(TTF_Font* font, const char* text);

		// The representation of a hardware texture
		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_texture;
		
		// raw pointer as creation and destruction of this is managed elsewhere. 
		SDL_Renderer* m_renderer;
	
		//Image dimensions
		int m_width;
		int m_height;
	};
}