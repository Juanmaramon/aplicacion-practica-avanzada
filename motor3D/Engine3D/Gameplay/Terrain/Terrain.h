/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006,2008 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

/* The original code was taken from bullet terrain tutorial */

#ifndef TERRAIN_H
#define TERRAIN_H

#include "..\..\Libraries\Bullet\include\btBulletCollisionCommon.h"
#include "..\..\Libraries\Bullet\include\btBulletDynamicsCommon.h"
#include "..\..\Libraries\Bullet\include\BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h"

#include "..\..\Physics\cPhysics.h"

// what type of terrain is generated?
enum eTerrainModel {
	eRadial			= 1,	// deterministic
	eFractal		= 2	// random
};

typedef unsigned char byte_t;

static const int s_gridSize			= 64 + 1;  // must be (2^N) + 1
static const float s_gridSpacing		= 5.0;

static const float s_gridHeightScale		= 0.2;


class Terrain {
public:

	// constructor, destructor ---------------------------------------------
	Terrain(void);
	~Terrain(void);

	virtual void initPhysics() {}

	// public class methods ------------------------------------------------
	void initialize(void);

private:

	// private helper methods ----------------------------------------------
//	void resetPhysics(void);
	void clearWorld(void);

	// private data members ------------------------------------------------
	int										m_upAxis;
	PHY_ScalarType							m_type;
	eTerrainModel							m_model;					
	byte_t *								m_rawHeightfieldData;		
	btScalar								m_minHeight;
	btScalar								m_maxHeight;

	btDynamicsWorld * 						m_dynamicsWorld;
};


// all we need to expose publicly is the factory method!

#endif //TERRAIN_H