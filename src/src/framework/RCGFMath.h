#pragma once

#include <cmath>

namespace math
{
	constexpr float PI = 3.14159265359f;

	struct Vec2
	{
		float x = 0;
		float y = 0;
		Vec2() = default;
		Vec2(float x, float y); 
		
		bool operator==(const Vec2& rhs);
		bool operator!=(const Vec2& rhs);
		Vec2& operator+=(const Vec2& rhs);
		Vec2& operator-=(const Vec2& rhs);
		Vec2 operator+(const Vec2& vec1) const;
		Vec2 operator-(const Vec2& vec1) const;
		Vec2 operator*(float scalar) const;
	};

	Vec2 rotate(const math::Vec2& src, float angle);                         // provide angle in radians (what range? might not matter)
	Vec2 scale(const Vec2& vec, float scale);
	float cross(const Vec2& vec1, const Vec2& vec2);
	float dot(const Vec2& vec1, const Vec2& Vec2);
	float angle(const Vec2& vec1, const Vec2& Vec2);  // returns range (0 to 2 * PI)
	float magnitude(const Vec2& vec);
	float distance(const Vec2& from, const Vec2& to);
	Vec2 angle_to_vec(float angle);
	float vec_to_angle(const Vec2& vec);              // returns range (-PI to PI)
	float vec_to_angle_pos(const Vec2& vec);          // returns range (0 to 2 * PI)
	Vec2 normalize(const Vec2& vec);
	float sum_angle(float angle1, float angle);       // returns range (0 to 2 * PI)

	struct Transform
	{
		math::Vec2 pos;
		float angle;
		Transform(math::Vec2 pos, float angle) : pos{ pos }, angle{ angle }{}
		Transform(float x, float y, float angle) : pos{ x, y }, angle{ angle }{}
	};
}