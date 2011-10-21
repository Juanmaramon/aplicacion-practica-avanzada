#include "cPhysicModel.h"

// Creates a new model using a Box Shape
void cPhysicModel::InitBox( float lfMass, const cVec3 &lHalfSize ){
	mfMass = lfMass;
	mpPhysicShape = cPhysics::Get( ).GetNewBoxShape( lHalfSize );
}

// Creates a new model using a Sphere Shape
void cPhysicModel::InitSphere( float lfMass, float lfRadius ){
	mfMass = lfMass;
	mpPhysicShape = cPhysics::Get( ).GetNewSphereShape( lfRadius );
}