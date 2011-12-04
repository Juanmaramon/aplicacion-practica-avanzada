#include "Character.h"
#include "..\Graphics\GraphicManager.h"
#include "Behaviour\BehaviourManager.h"
#include "..\LuaManager\cLuaManager.h"
#include <typeinfo.h>

//Método que inicializa a unos valores por defecto los atributos de la clase.
void cCharacter::Init(int id)
{
	mfYaw = 0.0f;
	mPosition = cVec3(0.0f,0.0f,0.0f);
	mfSpeed = 1.5f;
	mfAngSpeed = 1.0f;
	//Por defecto un personaje no tiene comportamiento
	mpActiveBehaviour = NULL;
	//Identificador del personaje
	miId = id;
}

void cCharacter::Deinit()
{ 
	//Desactiva el comportamiento asociado al personaje
	mpActiveBehaviour->Deinit();
	cBehaviourManager::Get().DestroyBehaviour(mpActiveBehaviour);
}

void cCharacter::Update( float lfTimestep )
{ 
	mpActiveBehaviour->Update(lfTimestep);
	//mPosition.x += .005f;
	//mPosition.z += .002f;
}

//Método que renderiza un punto que representa la posición del personaje y una 
// pequeña línea que parte de dicho punto que representa la orientación 
// del personaje.
void cCharacter::Render()
{
    /*La representación del personaje debe estar siempre en espacio local, es decir, que no 
	se debe tener en cuenta su orientación o posición. La posición y orientación del 
	personaje, se aplicarán a través de la matriz de World. De esta forma, cuando se use 
	una malla para representar al personaje, la IA seguirá funcionando correctamente. */

	//Componer la matriz de World
	cMatrix lWorld;
	lWorld.LoadIdentity();
	lWorld.LoadRotation(cVec3(0.0f, 1.0f, 0.0f), mfYaw);
	lWorld.SetPosition( mPosition );
	
	//Establecer la matriz de World
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	//Dibujar el personaje (un punto para indicar su posición y una 
	// línea para indicar su orientación) en espacio local. La  
	// orientación del personaje por defecto será mirando al eje Z 
	// positivo.
	cGraphicManager::Get().DrawPoint( cVec3(0.0f, 0.0f, 0.0f), cVec3(1.0f, 0.0f, 0.0f) );
	cGraphicManager::Get().DrawLine( cVec3(0.0f, 0.0f, 0.0f), cVec3(0.0f, 0.0f, 1.0f), cVec3(1.0f, 0.0f, 0.0f) );

	//Si el comportamiento del personaje es patrullero, pintamos su circuito.
	std::string lsTipo = typeid(*mpActiveBehaviour).name();	
	if (strcmp(lsTipo.c_str(), "class cPatrol") == 0)
	{
		//Llamamos a la función "CallLua" de "cLuaManager" para invocar a "DrawPath" de Lua.
		//Esta función renderiza el circuito (Path) que debe seguir el patrullero cuyo id
		// se pasa como parámetro.	
		//Esta función no retorna ningún valor, por eso se pone NULL en el último parámetro de "CallLua".		
		bool lbOk = cLuaManager::Get().CallLua( "DrawPath", miId, (int *) NULL);
		assert( lbOk );//Se comprueba si la llamada a la función se ha producido satisfactoriamente						
	}
	lWorld.LoadIdentity();
	//Reestablecer la matriz de World
	cGraphicManager::Get().SetWorldMatrix(lWorld);
}

//Método que establece el atributo de la posición del personaje
void cCharacter::SetPosition( const cVec3 &lPosition )
{
	mPosition = lPosition;
}

//Método que establece el atributo de la orientación del personaje (yaw).
void cCharacter::SetYaw( float lfYaw )
{
	mfYaw = lfYaw;      
}

//Método que establece el atributo de la velocidad máxima del personaje.
void cCharacter::SetSpeed( float lfSpeed )
{
	mfSpeed = lfSpeed;    
}

//Método que establece el atributo de la velocidad angular del personaje.
void cCharacter::SetAngSpeed( float lfAngSpeed )
{
	mfAngSpeed = lfAngSpeed;
}

//Método que establece el comportamiento del personaje.
void cCharacter::SetActiveBehaviour( cBehaviourBase * lpBehaviour ){
	mpActiveBehaviour = lpBehaviour;
	mpActiveBehaviour->Init(this);
}