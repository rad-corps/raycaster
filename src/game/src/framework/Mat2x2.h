#pragma once

namespace rcgf
{
	struct Mat2f
	{
		float elements[2][2];

		Mat2f MultiplyScalar(float scalar)
		{
			Mat2f ret;
			ret.elements[0][0] = elements[0][0] * scalar;
			ret.elements[0][1] = elements[0][1] * scalar;
			ret.elements[1][0] = elements[1][0] * scalar;
			ret.elements[1][1] = elements[1][1] * scalar;
			return ret;
		}
		Mat2f Invert()
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
	};
}