#ifndef Game_H
#define Game_H

#include "..\Utility\Singleton.h"
#include "..\Window\ApplicationProperties.h"
#include "..\Graphics\Camera.h"
#include "..\Graphics\Fonts\acgfx_font.h"
#include "..\Gameplay\Object\Object.h"
#include "..\Physics\cPhysics.h"
#include "..\Physics\cPhysicObject.h"
#include "..\Gameplay\Terrain\Heightmap.h"
#include "..\Gameplay\CharacterPos\CharacterPos.h"

//Clase que hace uso del Patrón Singleton definido en Singleton.h, para iniciar, actualizar, dibujar
// y finalizar el juego
class cGame : public cSingleton<cGame>
{
	friend class cSingleton<cGame>;
protected:
		cGame() { ; } // Protected constructor	
		//Variable privada para gestionar si tenemos que salir de la aplicación.
		bool mbFinish;
		//Clase para los atributos de la ventana (cWindow) que se creará en el método Init.
		cApplicationProperties mProperties;
        //Instancia de la clase cCamera que representa la camera de nuestro juego.
		cCamera m3DCamera;
		//Instancia de la clase cCamera. Será una cámara 2D para mostrar texto.
		cCamera m2DCamera;
		//Instancia de la clase cFont para crear texto.
		cFont mFont;
		//Instancia de la clase cResourceHandle que se encarga de manejar la escena del juego.
		cResourceHandle mScene;
		// Instance that manages skeletal animation part of the engine
		cResourceHandle mSkeletalMesh;
		// Bullet physics object
		//cPhysics mPhysics;
	
		// Terrain
		Heightmap mHeightmap;

public:
	
	//Función para inicializar el juego	
	bool Init();

	//Función para actualizar el juego
	void Update( float lfTimestep );

	//Función para renderizar el juego
	void Render();

	//Función para finalizar el juego
	bool Deinit();

	//Función que devuelve el valor de mbFinish, para chequear el final del juego.
	inline bool HasFinished() { return mbFinish; }

	//Función para cargar los recursos necesarios para el juego.
	bool LoadResources( void );

	// Time since application start
	inline float GetAcumulatedTime() { return mfAcTime; }	
	float mfAcTime;

	// Object for skeletal meshes
	cPhysicObject mObject;

	// Physic objects
	std::vector<cPhysicObject> maSphereObjects;
	cPhysicModel mSphereModel;
	cPhysicModel mBoxModel;
};

#endif