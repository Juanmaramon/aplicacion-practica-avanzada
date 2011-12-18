// Godmode Camera
#ifndef GODCAMERA_H
#define GODCAMERA_H

#include "Camera.h"

class GodCamera : public  cCamera{
public:
	void Init(); 
	void MoveFront(float lfTimestep);
	void MoveBack(float lfTimestep);
	void MoveLeft(float lfTimestep);
	void MoveRight(float lfTimestep);
	void MoveYawPitch(float lfYaw, float lfPitch, float lfTimestep);
	void Render(void);
	void ResetYawPitch(void){ mNew_pos = cVec3(0.f, 0.f, 0.f); }

private:
	cVec3 mOld_pos;
	cVec3 mNew_pos;
	cVec3 mPosition;
};

#endif 