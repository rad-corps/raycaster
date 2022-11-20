#pragma once

#include <cmath>

namespace math
{
	struct Mat2f
	{
		float elements[2][2];

		Mat2f MultiplyScalar(float scalar);
		
		Mat2f Invert();
	};

	struct Vec2f
	{
		float x = 0;
		float y = 0;
		Vec2f() = default;
		Vec2f(float x, float y); 

		void operator+=(const Vec2f& rhs);
	};

	float dot(const Vec2f& vec1, const Vec2f& vec2);
	float angle(const Vec2f& vec1, const Vec2f& vec2);
	float magnitude(const Vec2f& vec);
}