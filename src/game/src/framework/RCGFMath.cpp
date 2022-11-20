#include "RCGFMath.h"
#include <cmath>

namespace math
{
	
	Mat2f Mat2f::MultiplyScalar(float scalar)
	{
		Mat2f ret;
		ret.elements[0][0] = elements[0][0] * scalar;
		ret.elements[0][1] = elements[0][1] * scalar;
		ret.elements[1][0] = elements[1][0] * scalar;
		ret.elements[1][1] = elements[1][1] * scalar;
		return ret;
	}
	Mat2f Mat2f::Invert()
	{
		//transform sprite with the inverse camera matrix
		// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
		// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
		// [ planeY   dirY ]                                          [ -planeY  planeX ]

		// [ 0,0   0,1 ] -1                                       [ 1,1     -0,1 ]
		// [           ]       =  1/(planeX*dirY-dirX*planeY) *   [              ]
		// [ 1,0   1,1 ]                                          [ -1,0  0,0 ]
		Mat2f transposed;
		transposed.elements[0][0] = elements[1][1];
		transposed.elements[0][1] = -elements[0][1];
		transposed.elements[1][0] = -elements[1][0];
		transposed.elements[1][1] = elements[0][0];

		float scalar = 1 / (elements[0][0] * elements[1][1] - elements[0][1] * elements[1][0]);
		transposed = transposed.MultiplyScalar(scalar);
		return transposed;
	}
	

	Vec2f::Vec2f(float x, float y) : x{ x }, y{ y }{}

	void Vec2f::operator+=(const Vec2f& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	float dot(const Vec2f& vec1, const Vec2f& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}
	float angle(const Vec2f& vec1, const Vec2f& vec2)
	{
		//4100
		vec1;
		vec2;
		return 0.f;
	}

	float magnitude(const Vec2f& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}
}