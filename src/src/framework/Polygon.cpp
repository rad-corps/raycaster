#include "Polygon.h"
#include "Math.h"

namespace rcgf 
{
	Square::Square(SDL_Renderer* renderer, int len)
		: m_len{len}
		, m_renderer{renderer}
	{
		m_points[0] = math::Vec2{ 0.f, 0.f };
		m_points[1] = math::Vec2{ static_cast<float>(len), 0.f };
		m_points[2] = math::Vec2{ static_cast<float>(len), static_cast<float>(len) };
		m_points[3] = math::Vec2{ 0.f, static_cast<float>(len) };
	}

	//void Square::rotate(float rot)
	//{
	//	
	//}

	void Square::render(math::Vec2 pos)
	{
		std::array<math::Vec2, 4> world = m_points;
		for (int i = 0; i < 4; ++i)
		{
			world[i] += pos;
		}

		SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawLineF(m_renderer, world[0].x, world[0].y, world[1].x, world[1].y);
		SDL_RenderDrawLineF(m_renderer, world[1].x, world[1].y, world[2].x, world[2].y);
		SDL_RenderDrawLineF(m_renderer, world[2].x, world[2].y, world[3].x, world[3].y);
		SDL_RenderDrawLineF(m_renderer, world[3].x, world[3].y, world[0].x, world[0].y);
		
	}
}