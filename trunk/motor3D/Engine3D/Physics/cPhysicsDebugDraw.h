#ifndef cPhysicsDebugDraw_H
#define cPhysicsDebugDraw_H

#include "..\Libraries\Bullet\include\btBulletCollisionCommon.h"

//-----------
// Custom Debug Drawer for Bullet
//----------

class cPhysicsDebugDraw: public btIDebugDraw, public cSingleton<cPhysicsDebugDraw> {

public:
	cPhysicsDebugDraw() { ; }
	// Draw debug simplified shapes
	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
	// Draw contact point of collisions
	void drawContactPoint(const btVector3& lPointOnB, const btVector3& lNormalOnB, btScalar lfDistance, int lifeTime, const btVector3& lBtColor);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location,const char* textString);
	void setDebugMode(int liDebugMode) { miDebugMode=liDebugMode; }
	int getDebugMode() const { return miDebugMode; }
	friend class cSingleton<cPhysicsDebugDraw>;

private:
	int miDebugMode;
};

#endif