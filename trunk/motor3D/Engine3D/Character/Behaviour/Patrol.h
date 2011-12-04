//Clase  cPatrol, implementa un comportamiento de centinela.

#ifndef PATROL_H
#define PATROL_H

#include "..\Character.h"
#include "ChaserWithOrientation.h"
#include "BehaviourManager.h"

class cPatrol : public cBehaviourBase {

	private:
		cCharacter * mpCharacter; //Personaje que patrulla
		cVec3 mTargetWaypoint; //Siguiente punto objetivo
		cChaserWithOrientation* mpBehaviour;//Comportamiento perseguidor

	public:
		//Inicializa los atributos (mTargetPoint al mismo valor que la
		//posición del personaje)
		void Init(cCharacter * lpCharacter);
		//Deinicializa y destruye el comportamiento perseguidor
		void Deinit();
		//Implementa la máquina de estados del patrullero
		void Update(float lfTimestep);
		void SetTargetWaypoint( cVec3 lTargetWayPoint);
		inline const cVec3 &GetTargetWaypoint() const {return mTargetWaypoint;}
		void SetTarget(cVec3);
};

#endif