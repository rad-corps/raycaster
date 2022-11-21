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

	void Vec2::operator+=(const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	float dot(const Vec2& vec1, const Vec2& Vec2)
	{
		return vec1.x * Vec2.x + vec1.y * Vec2.y;
	}
	float angle(const Vec2& vec1, const Vec2& vec2)
	{
		return acosf(
			dot(vec1, vec2)
			/
			(magnitude(vec1) * magnitude(vec2))
		);
	}

	float magnitude(const Vec2& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	Vec2 angle_to_vec(float angle)
	{
		return Vec2{ cos(angle), sin(angle) };
	}
}