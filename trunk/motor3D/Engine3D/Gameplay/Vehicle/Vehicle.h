/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#ifndef VEHICLE_H
#define VEHICLE_H

#include "../../Libraries/Bullet/include/BulletDynamics/Vehicle/btRaycastVehicle.h"
#include "../../Mathlib/Mathlib.h"
#include "../../Libraries/Bullet/include/BulletCollision/CollisionShapes/btShapeHull.h"

#define PHYSCAR_SCALE 1
#define CUBE_HALF_EXTENTS 1

///Vehicle use the built-in raycast vehicle
class Vehicle{

public:

	btRigidBody* m_carChassis;
	btRaycastVehicle*	m_vehicle;
	btCollisionShape*	m_wheelShape;
	btVehicleRaycaster*	m_vehicleRayCaster;
	btRaycastVehicle::btVehicleTuning	m_tuning;
	class btCompoundShape* m_compound;
	btCollisionShape* m_chassisShape;

	Vehicle();
	~Vehicle(void);
	void initPhysics(void);
	void ResetVehicleParams();
	void renderme();
	void Update();
	void MoveForward(float lfTimestep);
	void Break(float lfTimestep);
	void SteeringLeft(float lfTimestep);
	void SteeringRight(float lfTimestep);

	cVec3 GetChasisPos(void);
	cVec3 GetChasisRot(void);
	float getEngineForce(void);
	void setEngineForce(float newForce);
	float getBreakingForce(void);
	void setBreakingForce(float newForce);

	/* añadido para depuracion de ruedas */
	struct ShapeCache{
		struct Edge { btVector3 n[2];int v[2]; };
		ShapeCache(btConvexShape* s) : m_shapehull(s) {}
		btShapeHull					m_shapehull;
		btAlignedObjectArray<Edge>	m_edges;
	};
	btAlignedObjectArray<ShapeCache*>	m_shapecaches;
	void debugWheels(btScalar* m, const btCollisionShape* shape, const btVector3& color,int	debugMode,const btVector3& worldBoundsMin,const btVector3& worldBoundsMax);
	ShapeCache*	cache(btConvexShape*);

};

#endif