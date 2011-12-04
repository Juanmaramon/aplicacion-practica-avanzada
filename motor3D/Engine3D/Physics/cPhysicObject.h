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
	void SetKinematic( );
	void SetPosition( const cVec3 &lPosition, float lYaw = 0.0f );
	cVec3 GetPosition( );
	void ReloadPhysicPosition(cMatrix &lRotationMatrix, cMatrix &lTranslationMatrix);
private:
	btRigidBody *mpPhysicBody;
	cMatrix mScaleMatrix;
	cMatrix mDrawOffsetMatrix;
	cVec3 mPosition;
};

#endif