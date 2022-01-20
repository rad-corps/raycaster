#include "Polygon.h"

namespace rcgf 
{
	Square::Square(int len)
		: m_len{len}
	{
		m_points[0] = glm::vec2{ 0.f, 0.f };
		m_points[1] = glm::vec2{ static_cast<float>(len), 0.f };
		m_points[2] = glm::vec2{ static_cast<float>(len), static_cast<float>(len) };
		m_points[3] = glm::vec2{ 0.f, static_cast<float>(len) };
	}

	//void Square::rotate(float rot)
	//{
	//	
	//}

	void Square::render(glm::vec2 pos)
	{
		std::array<glm::vec2, 4> world = m_points;
		for (int i = 0; i < 4; ++i)
		{
			world[i] += pos;
		}

		SDL_SetRenderDrawColor(global::instance.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawLineF(global::instance.getRenderer(), world[0].x, world[0].y, world[1].x, world[1].y);
		SDL_RenderDrawLineF(global::instance.getRenderer(), world[1].x, world[1].y, world[2].x, world[2].y);
		SDL_RenderDrawLineF(global::instance.getRenderer(), world[2].x, world[2].y, world[3].x, world[3].y);
		SDL_RenderDrawLineF(global::instance.getRenderer(), world[3].x, world[3].y, world[0].x, world[0].y);
		
	}
}