////////////////////////////////////////////////////////////////////////////
//	File: "Frustum.h"
//
//	Author: Brian Story
//
//	Creation Date: 9/5/2002
//
//	Purpose: Definition of the Frustrum class.
////////////////////////////////////////////////////////////////////////////


#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include <math.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <gl/gl.h>


///////////////////////////////////
//
//	Class CFrustrum
//
//	Created: 9/5/2002
//
///////////////////////////////////

class Frustum
{
	/////////////////////
	// Private Members //
	/////////////////////

	private:

	float frustum[6][4];		// The six sides of our frustum.



		//////////////////////////////////////////////////////////
		///	Function: "multMatrix"
		//
		///	FirstModified: 9/5/2002
		//
		///	\param float a[16]		The two matrices to be multiplied together.
		//		   float b[16]
		//		   float r[16]		The result of the multiplication.
		//
		///	Output: None
		//
		///	\return None
		//
		///	Purpose: Multiplies two 4x4 matrices together and returns the result.
		//////////////////////////////////////////////////////////
		void multMatrix(float a[16], float b[16], float r[16])
		{
			r[0] = (a[0] *b[0]) + (a[1] *b[4]) + (a[2] *b[8] ) + (a[3] *b[12]);
			r[1] = (a[0] *b[1]) + (a[1] *b[5]) + (a[2] *b[9] ) + (a[3] *b[13]);
			r[2] = (a[0] *b[2]) + (a[1] *b[6]) + (a[2] *b[10]) + (a[3] *b[14]);
			r[3] = (a[0] *b[3]) + (a[1] *b[7]) + (a[2] *b[11]) + (a[3] *b[15]);		
			r[4] = (a[4] *b[0]) + (a[5] *b[4]) + (a[6] *b[8] ) + (a[7] *b[12]);
			r[5] = (a[4] *b[1]) + (a[5] *b[5]) + (a[6] *b[9] ) + (a[7] *b[13]);
			r[6] = (a[4] *b[2]) + (a[5] *b[6]) + (a[6] *b[10]) + (a[7] *b[14]);
			r[7] = (a[4] *b[3]) + (a[5] *b[7]) + (a[6] *b[11]) + (a[7] *b[15]);		
			r[8] = (a[8] *b[0]) + (a[9] *b[4]) + (a[10]*b[8] ) + (a[11]*b[12]);
			r[9] = (a[8] *b[1]) + (a[9] *b[5]) + (a[10]*b[9] ) + (a[11]*b[13]);
			r[10]= (a[8] *b[2]) + (a[9] *b[6]) + (a[10]*b[10]) + (a[11]*b[14]);
			r[11]= (a[8] *b[3]) + (a[9] *b[7]) + (a[10]*b[11]) + (a[11]*b[15]);
			r[12]= (a[12]*b[0]) + (a[13]*b[4]) + (a[14]*b[8] ) + (a[15]*b[12]);
			r[13]= (a[12]*b[1]) + (a[13]*b[5]) + (a[14]*b[9] ) + (a[15]*b[13]);
			r[14]= (a[12]*b[2]) + (a[13]*b[6]) + (a[14]*b[10]) + (a[15]*b[14]);
			r[15]= (a[12]*b[3]) + (a[13]*b[7]) + (a[14]*b[11]) + (a[15]*b[15]);
		}


		//////////////////////////////////////////////////////////
		///	Function: "normPlane"
		//
		///	FirstModified: 9/24/2005
		//
		///	\param float plane[4]		The plane to normalize.
		//		   float result[4]		The resulting normalized plane.
		//
		///	Output: None
		//
		///	\return None
		//
		///	Purpose: Normalizes the given plane.
		//////////////////////////////////////////////////////////
		void normPlane(float plane[4], float result[4])
		{
			// Find the magnitude of the vector.
			double mag = sqrt((plane[0] * plane[0]) +
							  (plane[1] * plane[1]) +
							  (plane[2] * plane[2]));

			// Divide the values by the length.
			result[0] = float(plane[0] / mag);
			result[1] = float(plane[1] / mag);
			result[2] = float(plane[2] / mag);
			result[3] = float(plane[3] / mag);
		}

	////////////////////
	// Public Members //
	////////////////////

	public:

	/////////////////
	// Constructor //
	/////////////////

		//////////////////////////////////////////////////////////
		//	Function: CFrustrum
		//
		//	FirstModified: 9/5/2002
		//
		//	Input:	None
		//
		//	Output:	None
		//
		//	Returns:None
		//
		//	Purpose: Constructor for the class
		//////////////////////////////////////////////////////////
		Frustum(void){ }



	////////////////
	// Destructor //
	////////////////

		//////////////////////////////////////////////////////////
		//	Function: ~CFrustrum
		//
		//	FirstModified: 9/5/2002
		//
		//	Input:	None
		//
		//	Output:	None
		//
		//	Returns:None
		//
		//	Purpose: Destructor for the class
		//////////////////////////////////////////////////////////
		virtual ~Frustum(void){ }


		//////////////////////////////////////////////////////////
		///	Function: "
		//
		///	FirstModified: 9/5/2002
		//
		///	\param None
		//
		///	Output: None
		//
		///	\return None
		//
		///	Purpose: Calculates the sides of the frustum.
		//////////////////////////////////////////////////////////
		void calculateFrustum(void)
		{
			// Get the projection matrix.
			float projection[16];
			glGetFloatv(GL_PROJECTION_MATRIX, projection);

			// Get the modelview matrix.
			float modelview[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, modelview);


			// Before we can extract the sides of the frustum, we need to 
			// multiply the projection and modelview matrices together. 
			float result[16];
			multMatrix(modelview, projection, result);


			// Extract each side of the frustum and normalize that side.

			// Right side:
			frustum[0][0] = result[3]  - result[0];
			frustum[0][1] = result[7]  - result[4];
			frustum[0][2] =	result[11] - result[8];
			frustum[0][3] =	result[15] - result[12];

			normPlane(frustum[0], frustum[0]);

			// Left side:
			frustum[1][0] = result[3]  + result[0];
			frustum[1][1] = result[7]  + result[4];
			frustum[1][2] =	result[11] + result[8];
			frustum[1][3] =	result[15] + result[12];

			normPlane(frustum[1], frustum[1]);

			// Bottom:
			frustum[2][0] = result[3]  + result[1];
			frustum[2][1] = result[7]  + result[5];
			frustum[2][2] =	result[11] + result[9];
			frustum[2][3] =	result[15] + result[13];

			normPlane(frustum[2], frustum[2]);

			// Top:
			frustum[3][0] = result[3]  - result[1];
			frustum[3][1] = result[7]  - result[5];
			frustum[3][2] =	result[11] - result[9];
			frustum[3][3] =	result[15] - result[13];

			normPlane(frustum[3], frustum[3]);

			// Back:
			frustum[4][0] = result[3]  - result[2];
			frustum[4][1] = result[7]  - result[6];
			frustum[4][2] =	result[11] - result[10];
			frustum[4][3] =	result[15] - result[14];

			normPlane(frustum[4], frustum[4]);

			// Front:
			frustum[5][0] = result[3]  + result[2];
			frustum[5][1] = result[7]  + result[6];
			frustum[5][2] =	result[11] + result[10];
			frustum[5][3] =	result[15] + result[14];

			normPlane(frustum[5], frustum[5]);
		}


		//////////////////////////////////////////////////////////
		///	Function: "pointInFrustum"
		//
		///	FirstModified: 9/5/2002
		//
		///	\param float x		The x value of the point.
		//		   float y		The y value of the point.
		//		   float z		The z value of the point.
		//
		///	Output: None
		//
		///	\return True if the point is in the frustum.
		//
		///	Purpose: Checks to see if a point is in the frustum.
		//////////////////////////////////////////////////////////
		bool pointInFrustum(float x, float y, float z)
		{
			// Loop through the sides of the frustum.
			for (int i = 0; i < 6; i++)
			{
				// Calculate the distance to the side.
				float dist = frustum[i][0] * x +
							 frustum[i][1] * y +
							 frustum[i][2] * z +
							 frustum[i][3];

				// If the distance was negative, the point is outside the frustum.
				if (dist <= 0)
					return false;
			}

			// the point was inside the frustum.
			return true;
		}

};

#endif