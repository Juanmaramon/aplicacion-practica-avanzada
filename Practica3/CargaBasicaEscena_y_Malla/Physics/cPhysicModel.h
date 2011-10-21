#ifndef cPhysicModel_H
#define cPhysicModel_H

#include "..\MathLib\MathLib.h"
#include "..\Libraries\Bullet\include\btBulletCollisionCommon.h"
#include "cPhysics.h"

// This class links bullet with  the models inside the scene

class cPhysicModel{
public:
	void InitBox( float lfMass, const cVec3 &lHalfSize );
	void InitSphere( float lfMass, float lfRadius );
	btCollisionShape* GetShape( ) { return mpPhysicShape; }
	float GetMass( ) { return mfMass; }
private:
	btCollisionShape *mpPhysicShape;
	float mfMass;
};

#endif