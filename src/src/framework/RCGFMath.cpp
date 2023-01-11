#include "RCGFMath.h"
#include <cmath>

namespace math
{	
	Vec2::Vec2(float x, float y) : x{ x }, y{ y }{}

	Vec2& Vec2::operator+=(const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vec2& Vec2::operator-=(const Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vec2 Vec2::operator+(const Vec2& vec) const
	{
		Vec2 ret;
		ret.x = x + vec.x;
		ret.y = y + vec.y;
		return ret;
	}

	Vec2 Vec2::operator-(const Vec2& vec) const
	{
		Vec2 ret;
		ret.x = x - vec.x;
		ret.y = y - vec.y;
		return ret;
	}

	Vec2 Vec2::operator*(float scalar) const
	{
		Vec2 ret;
		ret.x = x * scalar;
		ret.y = y * scalar;
		return ret;
	}

	float dot(const Vec2& vec1, const Vec2& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	float angle(const Vec2& vec1, const Vec2& vec2)
	{
		float ret = atan2(vec2.y, vec2.x) - atan2(vec1.y, vec1.x);
		ret = ret < 0 ? ret + 2 * PI : ret;
		return ret;
	}

	float cross(const Vec2& vec1, const Vec2& vec2)
	{
		return (vec1.x * vec2.y) - (vec1.y * vec2.x);
	}

	float magnitude(const Vec2& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	float distance(const Vec2& from, const Vec2& to)
	{
		return magnitude(from - to);
	}

	Vec2 scale(const Vec2& vec, float scale)
	{
		Vec2 ret{vec.x * scale, vec.y * scale};
		return ret;
	}

	Vec2 angle_to_vec(float angle)
	{
		return Vec2{ cos(angle), sin(angle) };
	}

	float vec_to_angle(const Vec2& vec)
	{
		return atan2f(vec.y, vec.x);
	}

	float vec_to_angle_pos(const Vec2& vec)
	{
		float temp = vec_to_angle(vec);
		if (temp < 0.f)
		{
			temp = 2 * math::PI + temp;
		}
		return temp;
	}

	Vec2 normalize(const Vec2& vec)
	{
		float mag = magnitude(vec);
		return Vec2{ vec.x / mag, vec.y / mag };
	}

	float sum_angle(float angle1, float angle)
	{
		angle1 += angle;
		if (angle1 < 0) angle1 += PI * 2;
		else if (2 * PI < angle1) angle1 -= PI * 2;
		return angle1;
	}
	
	Vec2 rotate(const math::Vec2& src, float angle)
	{
		float currentAngle = vec_to_angle_pos(src);
		float newAngle = sum_angle(currentAngle, angle);
		return angle_to_vec(newAngle);
	}

}