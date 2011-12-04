//Clase  cChaserNoOrientation, implementa un comportamiento de perseguidor sin orientacion.

#ifndef CHASERNOORIENTATION_H
#define CHASERNOORIENTATION_H

#include "..\Character.h"

class cChaserNoOrientation : public cBehaviourBase {

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