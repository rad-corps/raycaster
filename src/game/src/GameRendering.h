#pragma once

#include "GameInclude.h"

namespace global
{
	class Global
	{
	public: 
		TTF_Font* getFont();
		SDL_Renderer* getRenderer();
		void setRenderer(SDL_Renderer*);
		void setFont(TTF_Font*);
	};

	extern Global instance;

}