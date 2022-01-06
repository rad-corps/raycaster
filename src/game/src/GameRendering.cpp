#include "GameRendering.h"

namespace
{
	TTF_Font* font;
	SDL_Renderer* renderer;
}

namespace global
{
	Global instance;
	TTF_Font* Global::getFont()
	{
		return font;
	}

	SDL_Renderer* Global::getRenderer()
	{
		return renderer;
	}

	void Global::setRenderer(SDL_Renderer* renderer_)
	{
		renderer = renderer_;
	}
	void Global::setFont(TTF_Font* font_)
	{
		font = font_;
	}

}