#include "GodCamera.h"
#include <windows.h> 
#include <stdio.h>
#include "GraphicManager.h"

#define MOUSE_SPEED 0.5f 

void GodCamera::Init(){
	mNew_pos  = cVec3(0.0f, 0.0f, 0.0f);
	mOld_pos = mNew_pos ;
	SetLookAt( cVec3(0.0f, 5.0f, -20.0f), cVec3(0.0f, 5.f, 0.f), cVec3(0.0f, 1.f, 0.f));
	mPosition = cVec3(0.0f, 5.0f, -20.0f);
}

void GodCamera::MoveFront(float lfTimestep){
	SetView(GetPosition() +  cVec3(0.f, 0.f, 10.f) * lfTimestep);
	mPosition += cVec3(0.f, 0.f, 10.f) * lfTimestep;
}

void GodCamera::MoveBack(float lfTimestep){
	SetView(GetPosition() - cVec3(0.f, 0.f, 10.f) * lfTimestep);
	mPosition -= cVec3(0.f, 0.f, 10.f) * lfTimestep;
}

void GodCamera::MoveLeft(float lfTimestep){
	SetView(GetPosition() + cVec3(10.f, 0.f, 0.f) * lfTimestep);
	mPosition += cVec3(10.f, 0.f, 0.f) * lfTimestep;
}

void GodCamera::MoveRight(float lfTimestep){
	SetView(GetPosition() - cVec3(10.f, 0.f, 0.f) * lfTimestep);
	mPosition -= cVec3(10.f, 0.f, 0.f) * lfTimestep;
}

void GodCamera::MoveYawPitch(float lfYaw, float lfPitch, float lfTimestep){
	// Chequea si no está inicializado el yaw y pitch
	float lfRotationY, lfRotationX;
	if (mNew_pos.IsZero()) {
		mOld_pos.x = lfPitch;
		mNew_pos.x = lfPitch;

		mOld_pos.y = lfYaw;
		mNew_pos.y = lfYaw;
	} else {
		lfRotationY = lfYaw - mOld_pos.y;
		lfRotationX = lfPitch - mOld_pos.x; 
		if (lfRotationY){
			SetYaw(lfRotationY * MOUSE_SPEED * lfTimestep);
		}
		if (lfRotationX){
			SetPitch(lfRotationX * MOUSE_SPEED * lfTimestep);
		}

		mOld_pos.y = lfYaw;
		mOld_pos.x = lfPitch;
	}
	char buf[250];
	sprintf(buf, " \n  rotationX(%2.2f) rotationY(%2.2f) Pitch(%2.2f), Yaw(%2.2f)\n", lfRotationX, lfRotationY, lfPitch, lfYaw);
	OutputDebugStr(buf);
}