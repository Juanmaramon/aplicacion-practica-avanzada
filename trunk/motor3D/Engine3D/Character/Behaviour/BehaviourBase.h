//Clase  cBehaviourBase, para gestionar el comportamiento de los personajes.

#ifndef BEHAVIOURBASE_H
#define BEHAVIOURBASE_H

//Evita la refencia cruzada entre ficheros de caberas (.h)
class cCharacter;
#include "..\..\MathLib\MathLib.h"

class cBehaviourBase
{

	public:
		//Inicializa los atributos del comportamiento y es llamada en el
		//momento en que se asocia este comportamiento a un personaje
		virtual void Init(cCharacter * lpCharacter) = 0;
		//Deinicializa los atributos del comportamiento y es llamada en el
		//momento en que se elimina este comportamiento de un personaje
		virtual void Deinit() = 0;
		//Esta función se llama cada frame para actualizar la lógica asociada
		//al comportamiento (Behaviour)
		virtual void Update(float lfTimestep) = 0;
		//Setea el objetivo
		virtual void SetTarget(cVec3 target) = 0;

	private:
		
	cCharacter * mpCharacter;	//Puntero al personaje
		
	cVec3 mTarget; //Posición del objetivo

};

#endif