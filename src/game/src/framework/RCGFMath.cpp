#include "RCGFMath.h"
#include <cmath>

namespace math
{
	
	Mat2 scale(const Mat2& mat, float scalar)
	{
		Mat2 ret;
		ret.elements[0][0] = mat.elements[0][0] * scalar;
		ret.elements[0][1] = mat.elements[0][1] * scalar;
		ret.elements[1][0] = mat.elements[1][0] * scalar;
		ret.elements[1][1] = mat.elements[1][1] * scalar;
		return ret;
	}
	Mat2 invert(const Mat2& mat)
	{
		//transform sprite with the inverse camera matrix
		// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
		// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
		// [ planeY   dirY ]                                          [ -planeY  planeX ]

		// [ 0,0   0,1 ] -1                                       [ 1,1     -0,1 ]
		// [           ]       =  1/(planeX*dirY-dirX*planeY) *   [              ]
		// [ 1,0   1,1 ]                                          [ -1,0  0,0 ]
		Mat2 transposed;
		transposed.elements[0][0] = mat.elements[1][1];
		transposed.elements[0][1] = -mat.elements[0][1];
		transposed.elements[1][0] = -mat.elements[1][0];
		transposed.elements[1][1] = mat.elements[0][0];

		float scalar = 1 / (mat.elements[0][0] * mat.elements[1][1] - mat.elements[0][1] * mat.elements[1][0]);
		transposed = scale(transposed, scalar);
		return transposed;
	}
	

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

	float dot(const Vec2& vec1, const Vec2& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

	float angle(const Vec2& vec1, const Vec2& vec2)
	{
		return acosf(dot(vec1, vec2) / (magnitude(vec1) * magnitude(vec2)));
	}

	float cross(const Vec2& vec1, const Vec2& vec2)
	{
		return (vec1.x * vec2.y) - (vec1.y * vec2.x);
	}

	float magnitude(const Vec2& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y);
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

	Vec2 normalize(const Vec2& vec)
	{
		float mag = magnitude(vec);
		return Vec2{ vec.x / mag, vec.y / mag };
	}
}