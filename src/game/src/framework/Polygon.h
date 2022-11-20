#pragma once

#include "Globals.h"
#include <array>
#include "RCGFMath.h"

namespace rcgf
{
	class Square
	{
	public:

		Square() = delete;
		Square(SDL_Renderer* renderer, int len);

		// void rotate(float rot);
		void render(math::Vec2f pos);

	private:
		int m_len;
		SDL_Renderer* m_renderer;
		
		// m_points in local space
		std::array<math::Vec2f, 4> m_points;
	};
}
