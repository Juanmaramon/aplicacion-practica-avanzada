//Clase  cChaserSnapOrientation, implementa un comportamiento de perseguidor hacia su objetivo.

#ifndef CHASERSNAPORIENTATION_H
#define CHASERSNAPORIENTATION_H

#include "..\Character.h"

class cChaserSnapOrientation : public cBehaviourBase {

	private:
		cCharacter * mpCharacter;	//Puntero al personaje
		cVec3 mTarget;				//Posición del objetivo

	public:
		//Inicializa los atributos
		void Init(cCharacter * lpCharacter);
		//Deinicializa los atributos
		void Deinit();
		//Implementa la variante del comportamiento perseguidor
		//correspondiente
		void Update(float lfTimestep);
		//Setea el objetivo
		void SetTarget(cVec3 target);
};

#endif