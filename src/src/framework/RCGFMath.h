#pragma once

#include <cmath>

namespace math
{
	constexpr float PI = 3.14159265359f;

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
		
		Vec2& operator+=(const Vec2& rhs);
		Vec2& operator-=(const Vec2& rhs);
		Vec2 operator+(const Vec2& vec1) const;
		Vec2 operator-(const Vec2& vec1) const;
		Vec2 operator*(float scalar) const;
		
	};

	
	Vec2 scale(const Vec2& vec, float scale);
	float cross(const Vec2& vec1, const Vec2& vec2);
	float dot(const Vec2& vec1, const Vec2& Vec2);
	float angle(const Vec2& vec1, const Vec2& Vec2);
	float magnitude(const Vec2& vec);
	Mat2 invert(const Mat2& mat);
	Mat2 scale(const Mat2& mat, float scalar);
	Vec2 angle_to_vec(float angle);
	Vec2 normalize(const Vec2& vec);
	float sum_angle(float angle1, float angle2);

	struct Transform
	{
		math::Vec2 pos;
		float angle;
		Transform(math::Vec2 pos, float angle) : pos{ pos }, angle{ angle }{}
		Transform(float x, float y, float angle) : pos{ x, y }, angle{ angle }{}
	};
}