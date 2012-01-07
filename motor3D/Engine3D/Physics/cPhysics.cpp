#include "cPhysics.h"
#include "cPhysicsDebugDraw.h"

cVec3 cPhysics::Bullet2Local( const btVector3& lFrom ){
	return cVec3( lFrom.x(), lFrom.y(), lFrom.z() );
}

cMatrix cPhysics::Bullet2Local( const btTransform& lFrom ){
	cMatrix lMatrix;
	lFrom.getOpenGLMatrix( &lMatrix.rows[ 0 ][ 0 ] );
	
	return lMatrix;
}

btVector3 cPhysics::Local2Bullet( const cVec3& lFrom ){
	return btVector3( lFrom.x, lFrom.y, lFrom.z );
}

btTransform cPhysics::Local2Bullet( const cMatrix& lFrom ){
	btTransform lBtTransform;
	lBtTransform.setFromOpenGLMatrix( &lFrom.rows[ 0 ][ 0 ] );
	
	return lBtTransform;
}

void cPhysics::Init( ){
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	mpCollisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	mpDispatcher = new btCollisionDispatcher(mpCollisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	mpOverlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	mpSolver = new btSequentialImpulseConstraintSolver;
	mpWorld = new btDiscreteDynamicsWorld(mpDispatcher, mpOverlappingPairCache, mpSolver, mpCollisionConfiguration);
	mpWorld->setGravity(btVector3(0,-10,0));

	///create a few basic rigid bodies
/*	btCollisionShape* lpGroundShape = new btBoxShape(btVector3(btScalar(10.),btScalar(10.),btScalar(10.)));
	mapCollisionShapes.push_back(lpGroundShape);
	GetNewBody( lpGroundShape, 0.0f, cVec3( 0.0f, -10.0f, 0.0f ));
	GetNewBody( lpGroundShape, 0.0f, cVec3( -20.0f, 0.0f, 0.0f ));
	GetNewBody( lpGroundShape, 0.0f, cVec3( 20.0f, 0.0f, 0.0f ));
	GetNewBody( lpGroundShape, 0.0f, cVec3( 0.0f, 0.0f, -20.0f ));
	GetNewBody( lpGroundShape, 0.0f, cVec3( 0.0f, 0.0f, 20.0f ));	*/


	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
	mapCollisionShapes.push_back(groundShape);
	GetNewBody( groundShape, 0.0f, cVec3( 0.0f, -5.0f, 0.0f ));	

	// Draws debug info of bullet
	cPhysicsDebugDraw::Get( ).setDebugMode( cPhysicsDebugDraw::DBG_DrawWireframe );
	mpWorld->setDebugDrawer( &cPhysicsDebugDraw::Get( ) );
}

void cPhysics::Deinit( ){
	//cleanup in the reverse order of creation/initialization
	//remove the rigidbodies from the dynamics world and delete them
	for ( int liIndex = mpWorld->getNumCollisionObjects() - 1; liIndex >= 0; liIndex-- ){
		btCollisionObject* obj = mpWorld->getCollisionObjectArray()[liIndex];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()){
			delete body->getMotionState();
		}
		mpWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for ( int liIndex =0; liIndex < mapCollisionShapes.size(); liIndex++){
		btCollisionShape* shape = mapCollisionShapes[liIndex];
		mapCollisionShapes[liIndex] = 0;
		delete shape;
	}

	//delete dynamics world
	delete mpWorld;

	//delete solver
	delete mpSolver;

	//delete broadphase
	delete mpOverlappingPairCache;

	//delete dispatcher
	delete mpDispatcher;
	delete mpCollisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	mapCollisionShapes.clear();
}

void cPhysics::Update( float lfTimestep ){
	mpWorld->stepSimulation( lfTimestep );
}

btCollisionShape* cPhysics::GetNewSphereShape( float lfRadius ){
	btCollisionShape* lShape = new btSphereShape( lfRadius );
	mapCollisionShapes.push_back( lShape );

	return lShape;
}

btCollisionShape* cPhysics::GetNewBoxShape( const cVec3& lHalfSize ){
	btCollisionShape* lShape = new btBoxShape( Local2Bullet( lHalfSize ) );
	mapCollisionShapes.push_back( lShape );

	return lShape;
}

btRigidBody* cPhysics::GetNewBody( btCollisionShape* lpShape, float lfMass, const cVec3& lPosition, float lRotation){
	/// Create Dynamic Objects
	btTransform lStartTransform;
	lStartTransform.setIdentity();

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool lbIsDynamic = (lfMass != 0.f);
	btVector3 lLocalInertia( 0, 0, 0);

	if ( lbIsDynamic ){
		lpShape->calculateLocalInertia(lfMass,lLocalInertia);
	}

	cMatrix lTransMatrix, lTransform;
	lTransMatrix.LoadTranslation(lPosition);
	lTransform = lTransMatrix;

	if (lRotation != 0.0f){
		lTransform.LoadIdentity();
			
		cMatrix lRotMatrix;
		lRotMatrix.LoadIdentity();
		lRotMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), lRotation);
		lTransform = lRotMatrix * lTransMatrix;
	}
	lStartTransform = Local2Bullet(lTransform);
	//lStartTransform.setOrigin( Local2Bullet( lPosition ) );

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* lpMotionState = new btDefaultMotionState( lStartTransform );
	btRigidBody::btRigidBodyConstructionInfo lInfo( lfMass, lpMotionState, lpShape, lLocalInertia);
	btRigidBody* lpBody = new btRigidBody( lInfo );
	mpWorld->addRigidBody(lpBody);

	return lpBody;
}

void cPhysics::Render( ){
	mpWorld->debugDrawWorld();
}