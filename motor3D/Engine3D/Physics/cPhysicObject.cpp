#include "cPhysicObject.h"

void cPhysicObject::Init(){
	cObject::Init( );
	mpPhysicBody = NULL;
	mScaleMatrix.LoadIdentity();
	mDrawOffsetMatrix.LoadIdentity();
}

void cPhysicObject::Deinit(){
	cObject::Deinit( );
}

// Must update world transform according to Physics
void cPhysicObject::Update( float lfTimestep ){
	cObject::Update( lfTimestep );
	if ( mpPhysicBody && mpPhysicBody->getMotionState( ) ) {
		// Our position depends on Bullet.
		btTransform lTransform;
		mpPhysicBody->getMotionState()->getWorldTransform(lTransform);
		SetWorldMatrix( mScaleMatrix * mDrawOffsetMatrix * cPhysics::Bullet2Local( lTransform ) );
	}
}

// Creates a New Physic Body and Link it to the object
void cPhysicObject::CreatePhysics( cPhysicModel* lpModel ){
	mpPhysicBody = cPhysics::Get( ).GetNewBody(lpModel->GetShape( ), lpModel->GetMass( ), mWorldMatrix.GetPosition( ) );
}

void cPhysicObject::SetKinematic( ){
	mpPhysicBody->setCollisionFlags( mpPhysicBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	mpPhysicBody->setActivationState( DISABLE_DEACTIVATION );
}

void cPhysicObject::SetPosition( const cVec3 &lPosition, float lYaw ){
	mPosition = lPosition;
	cMatrix lTranslationMatrix;
	lTranslationMatrix.LoadIdentity( );
	lTranslationMatrix.SetPosition( lPosition );
	// Rotation matrix
	cMatrix lRotationMatrix;
	lRotationMatrix.LoadRotation(cVec3(0.f, 1.f, 0.f), lYaw);
	// Change our own Matrix for drawing purposes
	SetWorldMatrix( lRotationMatrix * lTranslationMatrix );
	// Inform Bullet of the new Position
	if ( mpPhysicBody && mpPhysicBody->getMotionState( ) ) {
		mpPhysicBody->getMotionState()->setWorldTransform(cPhysics::Local2Bullet( lRotationMatrix * lTranslationMatrix ));
	}
}

void cPhysicObject::ReloadPhysicPosition(cMatrix &lRotationMatrix, cMatrix &lTranslationMatrix){
	// Inform Bullet of the new Position
	if ( mpPhysicBody && mpPhysicBody->getMotionState( ) ) {
		mpPhysicBody->getMotionState()->setWorldTransform(cPhysics::Local2Bullet( lRotationMatrix * lTranslationMatrix ));
	}
}

cVec3 cPhysicObject::GetPosition( ){
	return mPosition;
}