#include "Game.h"
#include "..\Window\Window.h"
#include "..\DebugClass\Debug.h"
#include "..\Graphics\GraphicManager.h"
#include "..\Input\InputConfiguration.h"
#include "..\Input\InputManager.h"
#include "..\Graphics\Textures\TextureManager.h" 
#include "..\Graphics\Meshes\MeshManager.h"
#include "..\Gameplay\Scene\Scene.h"
#include "..\Gameplay\Scene\SceneManager.h"
#include "..\Graphics\Materials\MaterialManager.h"
#include "..\Graphics\Effects\EffectManager.h"

//Para configurar el InputManager hay que llamar a su Init (en cGame::Init)
//pasándole la tabla kaActionMapping (de InputConfiguration.cpp).
extern tActionMapping kaActionMapping[];


//Función para inicializar el juego.
bool cGame::Init()
{	
	mbFinish = false;
	//	LoadResources();
	//Se rellena la estructura de tipo cApplicationProperties: 
	mProperties.macApplicationName = "Práctica 1: Shaders";
	mProperties.mbFullscreen = false;
	mProperties.muiBits = 16;
	mProperties.muiWidth = 640;
	mProperties.muiHeight = 480;	
	
	//Se inicializa el objeto ventana con la propiedades establecidas:
    bool lbResult = cWindow::Get().Init( mProperties );
		
	if ( lbResult )
	{
		//Se inicializa la clase que encapsula las operaciones de OpenGL:
		lbResult = cGraphicManager::Get().Init( &cWindow::Get() );   
		if (lbResult)
		{
			// Init Camera 3D
			m3DCamera.Init();  
			float lfAspect = (float)mProperties.muiWidth/(float)mProperties.muiHeight;
			m3DCamera.SetPerspective(45.0f, lfAspect,0.1f,100.0f);
			//Se pone la cámara en la posición (5,5,5) de nuestro mundo,
			// apuntando al origen del mismo, e indicando que el vector UP de la cámara apunta hacia arriba.
			//m3DCamera.SetLookAt( cVec3(5.0f, 5.f, 5.f), cVec3(0.0f, 0.f, 0.f), cVec3(0.0f, 1.f, 0.f) );		

			//Se aleja la cámara para ver bien la escena que vamos a cargar posteriormente.
			m3DCamera.SetLookAt( cVec3(5.f, 5.f, 5.f),cVec3(0.0f, 0.f, 0.f), cVec3(0.0f, 1.f, 0.f) );

			//Se inicializa la clase cInputManager que representa el gestor de entrada (keyboard, gamepad, mouse, ...).
			//Se le pasa la tabla "kaActionMapping" (de InputConfiguration.cpp) que indica la relación entre las acciones y los dispositivos.
			//También se pasa "eIA_Count" (de InputConfiguration.h) que indica cuantas acciones de entrada hay.
			cInputManager::Get().Init( kaActionMapping, eIA_Count );

			//Se inicializa la clase que gestiona la texturas indicando que habrá 1, por ejemplo.
			cTextureManager::Get().Init(10);

			//Se inicializa la clase que gestiona los materiales.
			cMaterialManager::Get().Init(10);

			// Init of effects management
			cEffectManager::Get().Init(1);

			//Init Camera 2D, para las cadenas de texto.
			//Se inicializa la cámara 2D con una perspectiva ortogonal.
			//El (0,0) está situado en el centro de la pantalla y las dimensiones están 
			// establecidas en píxeles.
			float lfRight = (float)mProperties.muiWidth / 2.0f;
			float lfLeft = -lfRight;
			float lfTop = (float)mProperties.muiHeight / 2.0f;
			float lfBottom = -lfTop;
			m2DCamera.Init();
			m2DCamera.SetOrtho(lfLeft, lfRight, lfBottom, lfTop, 0.1f, 100.0f);
			//Se pone la cámara2D en la posición (0,0) de nuestro mundo,
			// apuntando al origen del mismo, e indicando que el vector UP de la cámara apunta hacia arriba.
			m2DCamera.SetLookAt( cVec3(0.0f, 0.0f, 1.f), cVec3(0.0f, 0.f, 0.f), cVec3(0.0f, 1.f, 0.f) );

			// Init the Font.
			mFont.Init("./Data/Fonts/Test1.fnt");

			//Se inicializa el gestor de mallas: habrá 4 mallas en la escena (mirar ./Data/Scene/).
			cMeshManager::Get().Init(4);

			//Se inicializa el gestor de escenas.
			cSceneManager::Get().Init(10);         

			//Se carga la escena.
			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/dragonsmall.DAE" ); 			
		}		
		else
		{
			//Si algo falla se libera la ventana.
			cWindow::Get().Deinit();
		}
	}

	// Application start time
	mfAcTime = 0.0f;

    return lbResult;	
}

//Función para actualizar el juego.
void cGame::Update( float lfTimestep )
{
	//Se actualiza la ventana:
	cWindow::Get().Update();

	// Updates application duration time
	mfAcTime += lfTimestep;

	//Se actualiza el InputManager.
	cInputManager::Get().Update(lfTimestep);

	bool lbreloadEffect = IsPressed(eIA_ReloadEffectManager);
	if (lbreloadEffect) {
		cEffectManager::Get().Reload();
	}
	
	//Se comprueba si hay que cerrar la aplicación, por ejemplo a causa de 
	// que el usuario haya cerrado la ventana. 
	mbFinish = mbFinish || cWindow::Get().GetCloseApplication()
		         //También se verifica si se ha producido la acción de entrada que cierra la aplicación:
				 // Pulsar la tecla ESC del teclado, el botón 1 del JoyStick o el botón central del ratón.
	             || IsPressed(eIA_CloseApplication);
	//Si es así, se cambia el booleano
	// que controla el cierre de la aplicación.

	if ( mbFinish )
	{
		return;
	}	
}

//Función para renderizar el juego:
void cGame::Render()
{
	/* Orden de las instrucciones
	•    Limpiado de Buffers
	•    Activación de Cámara 3D
	•    Renderizado de Geometría 3D sólida
	•    Renderizado de Geometría 3D con transparencia
	•    Activación de Cámara 2D
	•    Renderizado de Cámara 2D
	•    Effectos de postprocesado
	•    Intercambio de los bufferes
	*/

	// 1) Limpiado de Buffers
	// -------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
	 
	// 2) Activación de Cámara 3D
	// -------------------------------------------------------------
	cGraphicManager::Get().ActivateCamera( &m3DCamera );
	 
	// 3) Renderizado de Geometría 3D sólida
	// -------------------------------------------------------------
	// Set the world matrix
	cMatrix lWorld;
	lWorld.LoadIdentity();
	cGraphicManager::Get().SetWorldMatrix(lWorld);
	// Render the debug lines
	cGraphicManager::Get().DrawGrid();
	cGraphicManager::Get().DrawAxis();
	 
	cGraphicManager::Get().DrawPoint( cVec3(1.5f, 0.0f, 1.5f), cVec3(1.0f, 0.0f, 1.0f) );
	cGraphicManager::Get().DrawLine( cVec3(-1.5f, 0.0f, -1.5f), cVec3(-1.5f, 0.0f, 1.5f), cVec3(1.0f, 1.0f, 0.0f) );

	//Desactivamos la textura porque aún no tenemos materiales asignados a las mallas de la escena.
//	glDisable(GL_TEXTURE_2D);
	//Para renderizar la escena llamaremos al render de la escena a través de mScene (manejador de la escena, de tipo cResourceHandle).
	((cScene *)mScene.GetResource())->Render();
//	glEnable(GL_TEXTURE_2D);
	 
	// 4) Renderizado de Geometría 3D con transparencia
	// -------------------------------------------------------------
	 
	// 5) Activación de Cámara 2D  
	// -------------------------------------------------------------
	cGraphicManager::Get().ActivateCamera( &m2DCamera );
	 
	// 6) Renderizado de Cámara 2D
	// -------------------------------------------------------------
	//Se dibujan algunas cadenas de texto.
	glEnable(GL_TEXTURE_2D);
	mFont.SetColour( 1.0f, 0.0f, 0.0f );
	mFont.Write(0,200,0, "Año Totó pingüino() ¡!¿?", 0, FONT_ALIGN_CENTER);
	 
	mFont.SetColour( 0.0f, 1.0f, 1.0f );
	mFont.WriteBox(100,100,0,100, "Esto es un test \nmultilinea", 0, FONT_ALIGN_CENTER);	


	 
	// 7) Efectos de postprocesado
	// -------------------------------------------------------------
	 
	// 8) Intercambio de los bufferes
	// -------------------------------------------------------------
	cGraphicManager::Get().SwapBuffer();
}

//Función para cargar los recursos necesarios para el juego.
bool cGame::LoadResources( void )
{
   return true;
}

//Función para finalizar el juego.
bool cGame::Deinit()
{
	//Se deinicializa en el orden inverso a la inicialización:

	cMaterialManager::Get().Deinit();

	//Se libera el manejador de escenas.
	cSceneManager::Get().Deinit();

	//Se libera el manejador de mallas.
	cMeshManager::Get().Deinit();

	//Se libera la clase fuente.
	mFont.Deinit();
    //Se libera el gestor de texturas.
	cTextureManager::Get().Deinit();    
	//Se libera el InputManager:
	cInputManager::Get().Deinit();
	//Se libera OpenGL (clase cGraphicManager):
	bool lbResult = cGraphicManager::Get().Deinit();
	//Se libera la ventana:
	lbResult = lbResult && cWindow::Get().Deinit();
	return lbResult;	
}