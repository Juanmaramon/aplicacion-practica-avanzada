//Clase  cChaserWithOrientation, implementa un comportamiento de perseguidor orientandose hacia su objetivo usando su velocidad angular.

#ifndef CHASERWITHORIENTATION_H
#define CHASERWITHORIENTATION_H

#include "..\Character.h"

class cChaserWithOrientation : public cBehaviourBase {

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
		//Devuelve true al alcanzar el punto objetivo
		bool EndPointReached();
};

#endif