#pragma once

#include "Globals.h"
#include <array>

namespace rcgf
{
	class Square
	{
	public:

		Square() = delete;
		Square(SDL_Renderer* renderer, int len);

		// void rotate(float rot);
		void render(glm::vec2 pos);

	private:
		int m_len;
		SDL_Renderer* m_renderer;
		
		// m_points in local space
		std::array<glm::vec2, 4> m_points;
	};
}
