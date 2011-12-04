#include "CharacterManager.h"
#include <assert.h>
#include <windows.h>

//Método de inicialización.
bool cCharacterManager::Init()
{
	return true;
}

//Método que crea un nuevo personaje, llama a su Init() y lo añade a la lista. 
//Devuelve un puntero al personaje creado.
cCharacter* cCharacterManager::CreateCharacter()
{
	//Reservamos memoria para un objeto de tipo cCharacter y obtenemos un puntero a dicho objeto
	cCharacter *lpCharacter = new cCharacter;

	//Nos aseguramos de que el puntero no es NULL
	assert(lpCharacter);

	int new_id = 1;
	//Calculamos el id del personaje
	for (cCharacterListIt lpIt= mCharacters.begin(); lpIt != mCharacters.end(); ++lpIt )
	{
		new_id++;
	}

	//Inicializamos el personaje
	lpCharacter->Init(new_id);

	//Añadimos a la lista un puntero al personaje recién creado
	mCharacters.push_back(lpCharacter);	

	//Devolvemos un puntero al personaje que acabamos de crear
	return lpCharacter;
}

//Método que elimina un personaje de la lista y llama a su Deinit(). 
void cCharacterManager::DestroyCharacter(cCharacter* lpCharacter)
{
	mCharacters.remove(lpCharacter);
	lpCharacter->Deinit();
}

//Método que llama al Update de todos los personajes.
void cCharacterManager::Update( float lfTimestep )
{
	//Se recorre la lista de personajes (mCharacters, contine varios objetos cCharacter) utilizando un 
	// iterador de tipo cCharacterListIt definido en el .h.
	//Se llama al método Update de cada cCharacter (personaje).
	for (cCharacterListIt lpIt= mCharacters.begin(); lpIt != mCharacters.end(); ++lpIt )
	{
		(*lpIt)->Update( lfTimestep );
	}
}

//Método que llama al Render de todos los personajes.
void cCharacterManager::Render()
{
	//Se recorre la lista de personajes (mCharacters, contine varios objetos cCharacter) utilizando un 
	// iterador de tipo cCharacterListIt definido en el .h.
	//Se llama al método Render de cada cCharacter (personaje).
	for (cCharacterListIt lpIt= mCharacters.begin(); lpIt != mCharacters.end(); ++lpIt )
	{
		(*lpIt)->Render();
	}
}

//Método que llama al Deinit de todos los personajes.
bool cCharacterManager::Deinit()
{
	//Se recorre la lista de personajes (mCharacters, contine varios objetos cCharacter) utilizando un 
	// iterador de tipo cCharacterListIt definido en el .h.
	//Se llama al método Deinit de cada cCharacter (personaje).
	for ( cCharacterListIt lpIt= mCharacters.begin(); lpIt != mCharacters.end(); ++lpIt )
	{
		(*lpIt)->Deinit();
		delete *lpIt;
	}
	//Se eliminan todos los personajes de las lista.
	mCharacters.clear();
	return true;
}

//Traza de los elementos de la lista de personajes
void cCharacterManager::DebugCharacter(){
	for ( cCharacterListIt lpIt = mCharacters.begin(); lpIt != mCharacters.end(); ++lpIt )
	{
		char temp[80];
		sprintf(temp, "%f", (*lpIt)->GetSpeed());
		OutputDebugStr(temp);
		sprintf(temp, "%d", (*lpIt)->GetId());
		OutputDebugStr(temp);
	}
}

//Se busca el personaje con Id igual a liCharacterId.
cCharacter* cCharacterManager::GetById(int hliCharacterId){
	//Se busca el personaje por Id
	for ( cCharacterListIt lpIt = mCharacters.begin(); lpIt != mCharacters.end(); ++lpIt )
	{
		if ((*lpIt)->GetId()){
			return *lpIt;
		}
	}	
	return false;
}