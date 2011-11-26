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

#include "Terrain.h"
#include <Windows.h>

Terrain::Terrain(void)
:
/*	m_collisionConfiguration(NULL),
	m_dispatcher(NULL),
	m_overlappingPairCache(NULL),
	m_constraintSolver(NULL),*/
	m_upAxis(1),
	m_type(PHY_FLOAT),
	m_model(eRadial),
	m_rawHeightfieldData(NULL)
/*	m_phase(0.0),
	m_isDynamic(true)		*/
{
}

Terrain::~Terrain(void){
	clearWorld();

	//delete dynamics world
/*	delete m_dynamicsWorld;

	//delete solver
	delete m_constraintSolver;

	//delete broadphase
	delete m_overlappingPairCache;

	//delete dispatcher
	delete m_dispatcher;

	delete m_collisionConfiguration;				*/
}

////////////////////////////////////////////////////////////////////////////////
//
//	Terrain -- public class methods
//
////////////////////////////////////////////////////////////////////////////////

static float convertToFloat(const byte_t * p, PHY_ScalarType type){
	btAssert(p);

	switch (type) {
	case PHY_FLOAT:
		{
			float * pf = (float *) p;
			return *pf;
		}

	case PHY_UCHAR:
		{
			unsigned char * pu = (unsigned char *) p;
			return ((*pu) * s_gridHeightScale);
		}

	case PHY_SHORT:
		{
			short * ps = (short *) p;
			return ((*ps) * s_gridHeightScale);
		}

	default:
		btAssert(!"bad type");
	}

	return 0;
}



static void convertFromFloat(byte_t * p, float value, PHY_ScalarType type){
	btAssert(p && "null");

	switch (type) {
	case PHY_FLOAT:
		{
			float * pf = (float *) p;
			*pf = value;
		}
		break;

	case PHY_UCHAR:
		{
			unsigned char * pu = (unsigned char *) p;
			*pu = (unsigned char) (value / s_gridHeightScale);
		}
		break;

	case PHY_SHORT:
		{
			short * ps = (short *) p;
			*ps = (short) (value / s_gridHeightScale);
		}
		break;

	default:
		btAssert(!"bad type");
	}
}

static float getGridHeight(byte_t * grid, int i, int j, PHY_ScalarType type){
	btAssert(grid);
	btAssert(i >= 0 && i < s_gridSize);
	btAssert(j >= 0 && j < s_gridSize);

	int bpe = sizeof(float);
	btAssert(bpe > 0 && "bad bytes per element");

	int idx = (j * s_gridSize) + i;
	long offset = ((long) bpe) * idx;

	byte_t * p = grid + offset;

	return convertToFloat(p, type);
}


// creates a radially-varying heightfield
static void setRadial(byte_t * grid, int bytesPerElement, PHY_ScalarType type, float phase = 0.0){
	btAssert(grid);
	btAssert(bytesPerElement > 0);

	// min/max
	float period = 0.5 / s_gridSpacing;
	float floor = 0.0;
	float min_r = 3.0 * sqrt(s_gridSpacing);
	float magnitude = 50.0 * sqrt(s_gridSpacing);

	// pick a base_phase such that phase = 0 results in max height
	//   (this way, if you create a heightfield with phase = 0,
	//    you can rely on the min/max heights that result)
	float base_phase = (0.5 * SIMD_PI) - (period * min_r);
	phase += base_phase;

	// center of grid
	float cx = 0.5 * s_gridSize * s_gridSpacing;
	float cy = cx;		// assume square grid
	byte_t * p = grid;
	for (int i = 0; i < s_gridSize; ++i) {
		float x = i * s_gridSpacing;
		for (int j = 0; j < s_gridSize; ++j) {
			float y = j * s_gridSpacing;

			float dx = x - cx;
			float dy = y - cy;

			float r = sqrt((dx * dx) + (dy * dy));

			float z = period;
			if (r < min_r) {
				r = min_r;
			}
			z = (1.0 / r) * sin(period * r + phase);
			if (z > period) {
				z = period;
			} else if (z < -period) {
				z = -period;
			}
			z = floor + magnitude * z;

			convertFromFloat(p, z, type);
			p += bytesPerElement;
		}
	}
}

static void dumpGrid(const byte_t * grid,int bytesPerElement,PHY_ScalarType type,int max){
	//std::cerr << "Grid:\n";

	char buffer[32];

	for (int j = 0; j < max; ++j) {
		for (int i = 0; i < max; ++i) {
			long offset = j * s_gridSize + i;
			float z = convertToFloat(grid + offset * bytesPerElement, type);
			sprintf(buffer, "%6.2f", z);
			//std::cerr << "  " << buffer;
		}
		//std::cerr << "\n";
	}
}

static byte_t * getRawHeightfieldData(eTerrainModel model, PHY_ScalarType type, btScalar& minHeight, btScalar& maxHeight){
//	std::cerr << "\nRegenerating terrain\n";
//	std::cerr << "  model = " << model << "\n";
//	std::cerr << "  type = " << type << "\n";

	long nElements = ((long) s_gridSize) * s_gridSize;
//	std::cerr << "  nElements = " << nElements << "\n";

	int bytesPerElement = sizeof(float);
//	std::cerr << "  bytesPerElement = " << bytesPerElement << "\n";
	btAssert(bytesPerElement > 0 && "bad bytes per element");

	long nBytes = nElements * bytesPerElement;
//	std::cerr << "  nBytes = " << nBytes << "\n";
	byte_t * raw = new byte_t[nBytes];
	btAssert(raw && "out of memory");

	// reseed randomization every 30 seconds
//	srand(time(NULL) / 30);

	// populate based on model
	setRadial(raw, bytesPerElement, type);

	if (0) {
		// inside if(0) so it keeps compiling but isn't
		// 	exercised and doesn't cause warnings
//		std::cerr << "final grid:\n";
		dumpGrid(raw, bytesPerElement, type, s_gridSize - 1);
	}

	// find min/max
	for (int i = 0; i < s_gridSize; ++i) {
		for (int j = 0; j < s_gridSize; ++j) {
			float z = getGridHeight(raw, i, j, type);
//			std::cerr << "i=" << i << ", j=" << j << ": z=" << z << "\n";

			// update min/max
			if (!i && !j) {
				minHeight = z;
				maxHeight = z;
			} else {
				if (z < minHeight) {
					minHeight = z;
				}
				if (z > maxHeight) {
					maxHeight = z;
				}
			}
		}
	}

	if (maxHeight < -minHeight) {
		maxHeight = -minHeight;
	}
	if (minHeight > -maxHeight) {
		minHeight = -maxHeight;
	}

//	std::cerr << "  minHeight = " << minHeight << "\n";
//	std::cerr << "  maxHeight = " << maxHeight << "\n";

	return raw;
}

static btVector3 getUpVector(int upAxis, btScalar regularValue, btScalar upValue){
	btAssert(upAxis >= 0 && upAxis <= 2 && "bad up axis");

	btVector3 v(regularValue, regularValue, regularValue);
	v[upAxis] = upValue;

	return v;
}


/// one-time class and physics initialization
void Terrain::initialize(void)
{
//	std::cerr << "initializing...\n";

	/* This code is inside cPhysics class */

	// set up basic state
/*	m_upAxis = 1;		// start with Y-axis as "up"
	m_type = PHY_FLOAT;
	m_model = eRadial;//eFractal;
	m_isDynamic = true;

	// set up the physics world
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	m_overlappingPairCache = new btAxisSweep3(worldMin,worldMax);
	m_constraintSolver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_overlappingPairCache,m_constraintSolver,m_collisionConfiguration);

	// initialize axis- or type-dependent physics from here
	this->resetPhysics();	*/

	clearWorld();

	// get new heightfield of appropriate type
	m_rawHeightfieldData = getRawHeightfieldData(eRadial, PHY_FLOAT, m_minHeight, m_maxHeight);

	bool flipQuadEdges = false;													// width, height, *heightmapData, scale, minHeight, maxHeight, upAxis, heightMapDatatype, flipQuadEdges 
	btHeightfieldTerrainShape * heightfieldShape = new btHeightfieldTerrainShape(s_gridSize, s_gridSize, m_rawHeightfieldData, s_gridHeightScale, m_minHeight, m_maxHeight, m_upAxis, PHY_FLOAT, flipQuadEdges);
	assert(heightfieldShape);

	// scale the shape
	btVector3 localScaling = getUpVector(m_upAxis, s_gridSpacing, 1.0);
	heightfieldShape->setLocalScaling(localScaling);

	// stash this shape away
	cPhysics::Get().getCollisionShapes().push_back(heightfieldShape);
	//m_collisionShapes.push_back(heightfieldShape);

	// set origin to middle of heightfield
	btTransform tr;
	tr.setIdentity();
	tr.setOrigin(btVector3(0,-20,0));

	// create ground object
	float mass = 0.0;
	//localCreateRigidBody(mass, tr, heightfieldShape);

   //We can also use DemoApplication::localCreateRigidBody, but for clarity it is provided here:
   
	btRigidBody* body = cPhysics::Get().GetNewBody(heightfieldShape, mass, cVec3(0, -20, 0));
}

/// removes all objects and shapes from the world
void Terrain::clearWorld(void)
{
	// delete raw heightfield data
	delete m_rawHeightfieldData;
	m_rawHeightfieldData = NULL;
}
