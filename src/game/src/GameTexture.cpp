#include "GameTexture.h"
#include <cassert>

namespace game
{
	Texture::Texture(SDL_Renderer* renderer, const char* pngPath)
		: m_width{0}, m_height{0}, m_renderer{renderer}, m_texture(nullptr, SDL_DestroyTexture)
	{
		//The final texture
		SDL_Texture* newTexture = NULL;

		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(pngPath);
		if (loadedSurface == NULL)
		{
			printf("Unable to load image %s! SDL_image Error: %s\n", pngPath, IMG_GetError());
		}
		else
		{
			//Color key image
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

			//Create texture from surface pixels
			newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
			if (newTexture == NULL)
			{
				printf("Unable to create texture from %s! SDL Error: %s\n", pngPath, SDL_GetError());
			}
			else
			{
				//Get image dimensions
				m_width = loadedSurface->w;
				m_height = loadedSurface->h;
			}

			//Get rid of old loaded surface
			SDL_FreeSurface(loadedSurface);
		}

		//Return success
		m_texture.reset(newTexture);
		assert(m_texture != nullptr);
	}

	Texture::Texture(SDL_Renderer* renderer, TTF_Font* font, const char* text)
		: m_width{ 0 }, m_height{ 0 }, m_renderer{ renderer }, m_texture(nullptr, SDL_DestroyTexture)
	{
		//Render text surface
		SDL_Color textColor{ 0xFF, 0xFF, 0xFF };
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
		if (textSurface == NULL)
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		}
		else
		{
			//Create texture from surface pixels
			m_texture.reset(SDL_CreateTextureFromSurface(m_renderer, textSurface));
			if (m_texture == nullptr)
			{
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				//Get image dimensions
				m_width = textSurface->w;
				m_height = textSurface->h;
			}

			//Get rid of old surface
			SDL_FreeSurface(textSurface);
		}

		//Return success
		assert(m_texture != nullptr);
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

	void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
	{
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { x, y, m_width, m_height };

		//Set clip rendering dimensions
		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		//Render to screen
		SDL_RenderCopyEx(m_renderer, m_texture.get(), clip, &renderQuad, angle, center, flip);
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