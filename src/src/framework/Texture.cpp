#include "Texture.h"
#include <cassert>
#include <iostream>

namespace rcgf
{
	Texture::Texture(SDL_Renderer* renderer, const char* pngPath)
		: m_renderer{renderer}, m_width{ 0 }, m_height{ 0 }, m_texture(nullptr, SDL_DestroyTexture), m_surface(nullptr, SDL_FreeSurface), m_name{ pngPath }
	{
		//The final texture
		SDL_Texture* newTexture = NULL;

		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(m_name.c_str());
		assert(loadedSurface != nullptr);

		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		// create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", m_name.c_str(), SDL_GetError());
		}
		else
		{
			// store image dimensions
			m_width = loadedSurface->w;
			m_height = loadedSurface->h;
		}
		

		//Return success
		m_texture.reset(newTexture);
		m_surface.reset(loadedSurface);
		assert(m_texture != nullptr);
	}

	Texture::Texture(SDL_Renderer* renderer, TTF_Font* font, const char* text)
		: m_renderer{ renderer }, m_width{ 0 }, m_height{ 0 }, m_texture(nullptr, SDL_DestroyTexture), m_surface(nullptr, SDL_FreeSurface), m_name{text}
	{
		SDL_Color textColor{ 0xFF, 0xFF, 0xFF };
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, m_name.c_str(), textColor);
		assert(textSurface != nullptr);
		m_surface.reset(textSurface);
		
		// create texture from surface pixels
		m_texture.reset(SDL_CreateTextureFromSurface(m_renderer, textSurface));
		if (m_texture == nullptr)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			// store image dimensions
			m_width = textSurface->w;
			m_height = textSurface->h;
		}
		assert(m_texture != nullptr);
	}

	void Texture::printDebugInfo()
	{
		SDL_LockSurface(m_surface.get());

		SDL_PixelFormat* fmt = m_surface->format;

		Uint32 pixel = *((Uint32*)m_surface->pixels);

		Uint8 rgba[4];

		SDL_GetRGBA(pixel, fmt, rgba, &rgba[1], &rgba[2], &rgba[3]);



		std::cout << "printDebugInfo: " << m_name << std::endl
			<< "Must Lock: " << SDL_MUSTLOCK(m_surface.get()) << std::endl
			<< "Bits Per Pixel: " << (int)fmt->BitsPerPixel << std::endl
			<< "Pixel: " << pixel << std::endl
			<< "r: " << (int)rgba[0] << std::endl
			<< "g: " << (int)rgba[1] << std::endl
			<< "b: " << (int)rgba[2] << std::endl
			<< "a: " << (int)rgba[3] << std::endl
			;

		SDL_UnlockSurface(m_surface.get());
	}

	SDL_Texture* Texture::get()
	{
		return m_texture.get();
	}

	void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
	{
		//Modulate texture rgb
		SDL_SetTextureColorMod(m_texture.get(), red, green, blue);
	}

	void Texture::setBlendMode(SDL_BlendMode blending)
	{
		//Set blending function
		SDL_SetTextureBlendMode(m_texture.get(), blending);
	}

	void Texture::setAlpha(Uint8 alpha)
	{
		//Modulate texture alpha
		SDL_SetTextureAlphaMod(m_texture.get(), alpha);
	}

	Color Texture::getPixelColor(int x, int y)
	{
		Color ret;

		// pitch is the length of a row of pixels in bytes
		Uint8* pixel = (Uint8*)m_surface->pixels + y * m_surface->pitch + x * m_surface->format->BytesPerPixel;
		Uint32* px32 = (Uint32*)pixel;

		SDL_GetRGBA(*px32, m_surface->format, &ret.r, &ret.g, &ret.b, &ret.a);

		return ret;
	}

	void Texture::render(int x, int y, SDL_Rect* clip, float scale, double angle, SDL_RendererFlip flip, bool drawFromCenter)
	{
		if ( drawFromCenter )
		{
			// find center then move x and y back from center
			x -= static_cast<int>(scale * m_width / 2);
			y -= static_cast<int>(scale * m_height / 2);
		}

		//Set rendering space and render to screen
		SDL_Rect renderQuad{ x, y, static_cast<int>(scale * m_width), static_cast<int>(scale * m_height) };

		//Set clip rendering dimensions
		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		//Render to screen
		SDL_RenderCopyEx(m_renderer, m_texture.get(), clip, &renderQuad, angle, NULL, flip);
	}
	
	void Texture::render2(const SDL_Rect* textureClip, const SDL_Rect* outputClip)
	{
		SDL_RenderCopyEx(m_renderer, m_texture.get(), textureClip, outputClip, 0.0, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	int Texture::getWidth()
	{
		return m_width;
	}

	int Texture::getHeight()
	{
		return m_height;
	}
}