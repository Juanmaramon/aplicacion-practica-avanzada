#ifndef cPhysics_H
#define cPhysics_H

// Class to encapsulate Bullet physics library 

#include "..\Utility\Singleton.h"
#include "..\MathLib\MathLib.h"
#include "..\Libraries\Bullet\include\btBulletDynamicsCommon.h"

class cPhysics : public cSingleton<cPhysics>{
public:
	cPhysics() { ; }
	void Init( );
	void Deinit( );
	void Update( float lfTimestep );
	// Creates basic shapes for collisions test
	btCollisionShape* GetNewSphereShape( float lfRadius );
	btCollisionShape* GetNewBoxShape( const cVec3& lHalfSize );
	btRigidBody* GetNewBody( btCollisionShape* lpShape, float lfMass, const cVec3& lPosition, float lRotation = 0.0f );
	btRigidBody* GetNewBody( btCollisionShape* lpShape, float lfMass, const cMatrix& lTranslation );
	// New for terrain generator
	btAlignedObjectArray<btCollisionShape*>  getCollisionShapes(){ return mapCollisionShapes; }

	// Static Methods to translate from and to Bullet Variable Types
	static cVec3 Bullet2Local( const btVector3& lFrom );
	static cMatrix Bullet2Local( const btTransform& lFrom );
	static btVector3 Local2Bullet( const cVec3& lFrom );
	static btTransform Local2Bullet( const cMatrix& lFrom );
	friend class cSingleton<cPhysics>;

	// Draws debug info
	void Render( );

	// Devuelve mundo Bullet
	btDiscreteDynamicsWorld* GetBulletWorld(){ return mpWorld; };

private:
	// Bullet world
	btDiscreteDynamicsWorld* mpWorld;
	btDefaultCollisionConfiguration* mpCollisionConfiguration;
	btCollisionDispatcher* mpDispatcher;
	btBroadphaseInterface* mpOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mpSolver;
	btAlignedObjectArray<btCollisionShape*> mapCollisionShapes;
};

#endif