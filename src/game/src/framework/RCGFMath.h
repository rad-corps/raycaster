#pragma once

#include <cmath>

namespace math
{
	struct Mat2
	{
		float elements[2][2];
	};

	struct Vec2
	{
		float x = 0;
		float y = 0;
		Vec2() = default;
		Vec2(float x, float y); 

		void operator+=(const Vec2& rhs);
	};

	float dot(const Vec2& vec1, const Vec2& Vec2);
	float angle(const Vec2& vec1, const Vec2& Vec2);
	float magnitude(const Vec2& vec);
	Mat2 invert(const Mat2& mat);
	Mat2 scale(const Mat2& mat, float scalar);
	Vec2 angle_to_vec(float);
}