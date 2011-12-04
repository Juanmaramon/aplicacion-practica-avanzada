#include "LuaFunctions.h"
#include "../LuaManager/cLuaManager.h"
#include "../Character/Character.h"
#include "../Character/CharacterManager.h"
#include "../Character/Behaviour/BehaviourManager.h"
#include "../Graphics/GraphicManager.h"

//Esta función será llamada desde Lua. Se encargará de crear un personaje y asignarle un comportamiento.
//Además, inicializará el objetivo de este personaje, establecerá su velocidad máxima, velocidad angular.
//Por último, devolverá el identificador del personaje creado.
int CreatePatrol(lua_State* lpLuaContext){

	//Comprobamos que el contexto de Lua no es NULL
	assert( lpLuaContext );

	//Cogemos el numero de argumentos de la pila de Lua
	int liArgCount = lua_gettop( lpLuaContext );

	//Comprobamos que recibimos cinco argumentos
	assert( liArgCount == 5 );

	//Cogemos los argumentos de la pila
	float liPosX = luaL_checknumber( lpLuaContext,1 );		//1º argumento
	float liPosY = luaL_checknumber( lpLuaContext,2 );		//2º argumento
	float liPosZ = luaL_checknumber( lpLuaContext,3 );		//3º argumento
	float liSpeed = luaL_checknumber( lpLuaContext,4 );		//4º argumento
	float liAngSpeed = luaL_checknumber( lpLuaContext,5 );	//5º argumento
	
	//Se crea un personaje.
	cCharacter *lpCharacterPrueba = cCharacterManager::Get().CreateCharacter();	

	//Se establece su posición.
	lpCharacterPrueba->SetPosition( cVec3(-4.0f, 0.0f, -2.0f) );
	//Se establece su velocidad máxima y angular.
	lpCharacterPrueba->SetSpeed(liSpeed);
	lpCharacterPrueba->SetAngSpeed(liAngSpeed);

	//Se crea el comportamiento de perseguidor.
	cBehaviourBase *lpBehaviour = cBehaviourManager::Get().CreateBehaviour(cBehaviourManager::ePATROL);
	//Se incializa el comportamiento.
	//lpBehaviour->Init(lpCharacterPrueba);

   //Se asigna el comportamiento al personaje.
   lpCharacterPrueba->SetActiveBehaviour(lpBehaviour);

	int liId = lpCharacterPrueba->GetId();
	//Introducimos en la pila el identificador del personaje
	lua_pushinteger( lpLuaContext, liId );
	
	//Devolvemos el numero de valores de retorno
	//que hemos introducido en la pila
	return 1;
}

//Esta función será llamada desde Lua. Se encargará de cambiar el objetivo al siguiente punto
//para el personaje cuyo id sea igual a CharacterId. 
int SetPatrolTarget(lua_State* lpLuaContext ){
	//Comprobamos que el contexto de Lua no es NULL
	assert( lpLuaContext );

	//Cogemos el numero de argumentos de la pila de Lua
	int liArgCount = lua_gettop( lpLuaContext );

	//Comprobamos que recibimos cinco argumentos
	assert( liArgCount == 4 );

	//Cogemos los argumentos de la pila
	int liCharacterId = luaL_checknumber( lpLuaContext,1 );		//1º argumento
	float lfPosX = luaL_checknumber( lpLuaContext,2 );				//2º argumento
	float lfPosY = luaL_checknumber( lpLuaContext,3 );				//3º argumento
	float lfPosZ = luaL_checknumber( lpLuaContext,4 );				//4º argumento
	
	//Se busca el personaje con Id igual a liCharacterId.
	cCharacter *lpCharacterPrueba = cCharacterManager::Get().GetById(liCharacterId);	

	assert(lpCharacterPrueba);

	//Se establece el siguiente punto a perseguir (mTargetWayPoint).
	//Se convierte el padre (cBehaviourBase) al hijo (cPatrol).
	//El método SetTargetWaypoint es propio de cPatrol.
	( (cPatrol *)lpCharacterPrueba->GetActiveBehaviour() )->SetTargetWaypoint(cVec3(lfPosX, lfPosY, lfPosZ));

	int liId = lpCharacterPrueba->GetId();
	//Introducimos en la pila el identificador del personaje
	lua_pushinteger( lpLuaContext, liId );
	
	//Devolvemos el numero de valores de retorno
	//que hemos introducido en la pila
	return 1;
}


//Función que dibuja de un determinado color la línea 
// que une dos puntos (a,b) del circuito de un patrullero.
//Será llamada desde Lua de la siguiente manera:
//  DrawLine(lfPosXa, lfPosYa, lfPosZa, lfPosXb, lfPosYb, lfPosZb)
//Lua hará tantas llamadas a esta función como sea neceario para renderiazar todo el circuito.
int DrawLine( lua_State* lpLuaContext )
{
	//Comprobamos que el contexto de Lua no es NULL
	assert( lpLuaContext );

	//Cogemos el numero de argumentos de la pila de Lua
	int liArgCount = lua_gettop( lpLuaContext ); 

	//Comprobamos que recibimos 6 argumentos (lfPosXa, lfPosYa, lfPosZa, lfPosXb, lfPosYb, lfPosZb)
	assert( liArgCount == 6 );

	//Cogemos los argumentos de la pila   
	float lfPosXa = (float)luaL_checknumber( lpLuaContext,1 ); //1º argumento
	float lfPosYa = (float)luaL_checknumber( lpLuaContext,2 ); //2º argumento
	float lfPosZa = (float)luaL_checknumber( lpLuaContext,3 ); //3º argumento 
	float lfPosXb = (float)luaL_checknumber( lpLuaContext,4 ); //4º argumento
	float lfPosYb = (float)luaL_checknumber( lpLuaContext,5 ); //5º argumento
	float lfPosZb = (float)luaL_checknumber( lpLuaContext,6 ); //6º argumento             

	// Set the world matrix
	cMatrix lWorld;
	lWorld.LoadIdentity();
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	//Dibujamos los dos puntos
    cGraphicManager::Get().DrawPoint( cVec3(lfPosXa, lfPosYa, lfPosZa), cVec3(-1.0f, 0.0f, 1.0f) );
	cGraphicManager::Get().DrawPoint( cVec3(lfPosXb, lfPosYb, lfPosZb), cVec3(-1.0f, 0.0f, 1.0f) );
	//Dibujamos la línea que une los dos puntos
	cGraphicManager::Get().DrawLine( cVec3(lfPosXa, lfPosYa, lfPosZa), cVec3(lfPosXb, lfPosYb, lfPosZb), cVec3(1.0f, 0.0f, 1.0f) );

	//Devolvemos el número de valores de retorno
	//que hemos introducido en la pila
	return 0;
}

//Función que registrará en Lua las funciones C++
void RegisterLuaFunctions(){
	cLuaManager::Get().Register("CreatePatrol", CreatePatrol);
	cLuaManager::Get().Register("SetPatrolTarget", SetPatrolTarget);
	cLuaManager::Get().Register("DrawLine", DrawLine);
}