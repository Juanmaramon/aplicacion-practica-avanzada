#ifndef cPhysicObject_H
#define cPhysicObject_H

#include "..\Gameplay\Object\Object.h"
#include "cPhysicModel.h"
#include "..\MathLib\MathLib.h"

class cPhysicObject : public cObject{
public:
	virtual void Init();
	virtual void Deinit();
	virtual void Update( float lfTimestep );
	void CreatePhysics( cPhysicModel* lpModel );
	inline void SetScaleMatrix(const cMatrix& lScale){ mScaleMatrix = lScale; }
	inline cMatrix GetScaleMatrix( ) { return mScaleMatrix; }
	inline void SetDrawOffsetMatrix(const cMatrix& lOffset){ mDrawOffsetMatrix = lOffset; }
	inline cMatrix GetDrawOffsetMatrix( ) { return mDrawOffsetMatrix; }
private:
	btRigidBody *mpPhysicBody;
	cMatrix mScaleMatrix;
	cMatrix mDrawOffsetMatrix;
};

#endif