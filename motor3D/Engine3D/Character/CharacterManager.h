//Clase cCharacterManager que se encargará de gestionar los 
// personajes del juego (instancias de cCharacter). 
//Esta clase es un Singleton.
#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include <list>

#include "Character.h"
//class cCharacter; //Escribimos esto en lugar de colocar #include "Character.h". 
				 //Esto lo podemos hacer en aquellos .h en los que se van a declarar punteros a otra clase 
				 // que no van a ser usados en dicho .h. Es una forma de prevenir problemas de referencias cruzadas más tarde.

#include "..\Utility\Singleton.h"

//Esta clase usará el patrón Singleton definido en "..Utility/Singleton.h"
class cCharacterManager : public cSingleton<cCharacterManager>
{
	private:
		//Tipo de dato que representa una lista de objetos de la clase cCharacter.
		typedef std::list<cCharacter *> cCharacterList;

		//Tipo de dato que representa un iterador para recorrer una lista del tipo cCharacterList,
		// definido en la línea anterior.
		typedef cCharacterList::iterator cCharacterListIt;

		cCharacterList mCharacters; //Lista de personajes de tipo cCharacterList. 

	protected:
		cCharacterManager() { ; } //Constructor

	public:
		friend class cSingleton<cCharacterManager>;

		//Devuelve true.
		bool Init(); 

		//Llama al Update de todos los personajes.
		void Update( float lfTimestep );
		
		//Llama al Render de todos los personajes.
		void Render();
		
		//Llama al Deinit de todos los personajes.
		bool Deinit();
		
		//Crea un nuevo personaje, llama a su Init() y lo añade a la lista. 
		//Devuelve un puntero al personaje creado.
		cCharacter* CreateCharacter();
		
		//Elimina al personaje de la lista y llama a su Deinit(). 
		void DestroyCharacter(cCharacter* lpCharacter);

		//Traza de los elementos de la lista de personajes
		void DebugCharacter();

		//Se busca el personaje con Id igual a liCharacterId.
		cCharacter* GetById(int liCharacterId);	
};

#endif