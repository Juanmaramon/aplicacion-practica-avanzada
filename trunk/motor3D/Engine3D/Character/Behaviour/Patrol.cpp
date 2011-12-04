#include "Patrol.h"
#include <string>
#include "../../LuaManager/cLuaManager.h"
//#include <windows.h>

//Inicializa los atributos
void cPatrol::Init(cCharacter * lpCharacter){
	mpCharacter = lpCharacter;
	mTargetWaypoint = cVec3(0.0f, 0.0f, 0.0f);
	mpBehaviour = (cChaserWithOrientation *) cBehaviourManager::Get().CreateBehaviour(cBehaviourManager::eCHASER_WITH_ORIENTATION);
	mpBehaviour->Init(mpCharacter);
}

//Deinicializa los atributos
void cPatrol::Deinit(){
	mpCharacter = NULL;
	mpBehaviour->Deinit();
	cBehaviourManager::Get().DestroyBehaviour(mpBehaviour);
}

//Implementa la variante del comportamiento perseguidor
//correspondiente
void cPatrol::Update(float lfTimestep){

	//Se setea el objetivo del comportamiento
	mpBehaviour->SetTarget(mTargetWaypoint);
	//Se llama a la funcion update del comportamiento
	mpBehaviour->Update(lfTimestep);
	//Si se alcanza el punto pedir el siguiente
	if (mpBehaviour->EndPointReached()){
		bool lbOk = cLuaManager::Get().CallLua( "NextEndPoint",mpCharacter->GetId(),(int*) NULL );
		assert(lbOk);
	}
}

//Setea el objetivo
void cPatrol::SetTargetWaypoint(cVec3 lTargetWayPoint){
	mTargetWaypoint = lTargetWayPoint;
}

void cPatrol::SetTarget(cVec3){;}