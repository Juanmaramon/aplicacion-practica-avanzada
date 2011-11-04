#include "..\Graphics\GraphicManager.h"
#include "cPhysicsDebugDraw.h"
#include "cPhysics.h"

void cPhysicsDebugDraw::drawLine(const btVector3& lFrom,const btVector3& lTo,const btVector3& lColor ){
	cGraphicManager::Get().DrawLine( cPhysics::Bullet2Local( lFrom ), cPhysics::Bullet2Local( lTo ), cPhysics::Bullet2Local( lColor ) );
}

void cPhysicsDebugDraw::drawContactPoint(const btVector3& lPointOnB, const btVector3& lNormalOnB, btScalar lfDistance, int lifeTime, const btVector3& lBtColor){
	cVec3 lColor = cPhysics::Bullet2Local( lBtColor );
	cVec3 lFrom = cPhysics::Bullet2Local( lPointOnB );
	cVec3 lTo = lFrom + cPhysics::Bullet2Local( lNormalOnB ) * lfDistance;
	cGraphicManager::Get().DrawPoint( lFrom, lColor );
	cGraphicManager::Get().DrawLine( lFrom, lTo, lColor );
}

void cPhysicsDebugDraw::reportErrorWarning(const char* warningString){
}

void cPhysicsDebugDraw::draw3dText(const btVector3& location,const char* textString){
// Usar la clase cFont
}