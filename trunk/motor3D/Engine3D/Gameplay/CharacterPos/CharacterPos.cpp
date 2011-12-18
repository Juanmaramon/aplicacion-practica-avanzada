#include "CharacterPos.h"

void CharacterPos::Init(cVec3 lPosIni, float lYawIni, int lVel, float lAngVel){
	mPosition = lPosIni;
	mYaw = lYawIni;
	mVelocity = lVel;
	mAngVel = lAngVel;
	mPosDirChange = false;
	mDisplacement = 0.0f;
}

void CharacterPos::DeInit(void){}

void CharacterPos::SetCharacterPosition(cVec3 lPos){
	mPosition = lPos;
	mPosDirChange = true;
}

void CharacterPos::SetCharacterYaw(float lYaw){
	mYaw = lYaw;
	mPosDirChange = true;
}

void CharacterPos::SetVelocity(int lVel){
	mVelocity = lVel;
}

void CharacterPos::MoveFront(){
	mPosition += GetFront() * mDisplacement;
	mPosDirChange = true;
}

void CharacterPos::MoveBack(void){
	mPosition -= GetFront() * mDisplacement;
	mPosDirChange = true;
}

void CharacterPos::TurnRight(void){
	mYaw -= mAngDisplacement; 	
}

void CharacterPos::TurnLeft(void){
	mYaw += mAngDisplacement; 	
}

void CharacterPos::Update(float lfTimestep){
	mDisplacement = mVelocity * lfTimestep;
	mAngDisplacement = mAngVel * lfTimestep;
}