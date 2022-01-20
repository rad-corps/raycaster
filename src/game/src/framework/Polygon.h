#pragma once

#include "Globals.h"
#include <array>

namespace rcgf
{
	class Square
	{
	public:

		Square() = delete;
		Square(int len);

		// void rotate(float rot);
		void render(glm::vec2 pos);

	private:
		int m_len;
		
		// m_points in local space
		std::array<glm::vec2, 4> m_points;
	};
}
