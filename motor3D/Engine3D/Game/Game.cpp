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
#include "..\Graphics\Skeletal\cSkeletalManager.h"
#include "..\Graphics\Skeletal\cSkeletalMesh.h"
#include "..\Character\CharacterManager.h"
#include "..\Character\Behaviour\BehaviourManager.h"
#include "..\Lua\LuaFunctions.h"
#include "..\LuaManager\cLuaManager.h"

//Para configurar el InputManager hay que llamar a su Init (en cGame::Init)
//pasándole la tabla kaActionMapping (de InputConfiguration.cpp).
extern tActionMapping kaActionMapping[];

//Función para inicializar el juego.
bool cGame::Init()
{	
	mbFinish = false;
	//	LoadResources();
	//Se rellena la estructura de tipo cApplicationProperties: 
	mProperties.macApplicationName = "Proyecto fin master";
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
			m3DCamera.SetPerspective(45.0f, lfAspect,0.1f,1000.0f);
			//Se pone la cámara en la posición (5,5,5) de nuestro mundo,
			// apuntando al origen del mismo, e indicando que el vector UP de la cámara apunta hacia arriba.
			//m3DCamera.SetLookAt( cVec3(5.0f, 5.f, 5.f), cVec3(0.0f, 0.f, 0.f), cVec3(0.0f, 1.f, 0.f) );		

			//Se aleja la cámara para ver bien la escena que vamos a cargar posteriormente.
			//m3DCamera.SetLookAt( cVec3(5.f, 4.3f, 5.f),cVec3(0.f, 0.f, 0.f), cVec3(0.0f, 1.f, 0.f) );
			m3DCamera.SetLookAt( cVec3(0.f, 1.5f, 20.f),cVec3(0.f, 1.5f, 0.f), cVec3(0.0f, 1.f, 0.f) );

			glClearColor(1.0f, 0.9f, 0.7f, 1.0f);

			//Se inicializa la clase que contendrá la lista de personajes.
			lbResult = cCharacterManager::Get().Init();
			//Se inicializa la clase que contendrá los comportamientos de los personajes.
			lbResult = lbResult && cBehaviourManager::Get().Init();
			if (lbResult)
			{
				//Se inicializa Lua.
				cLuaManager::Get().Init();
				//Se registran las funciones C++ para usar desde Lua.
				RegisterLuaFunctions();
				//Cargamos el fichero de script
				cLuaManager::Get().DoFile("Data/Scripts/character.lua");
				cCharacterManager::Get().DebugCharacter();
				cLuaManager::Get().DoFile("Data/Scripts/patrol.lua");
			}

			//Se inicializa la clase cInputManager que representa el gestor de entrada (keyboard, gamepad, mouse, ...).
			//Se le pasa la tabla "kaActionMapping" (de InputConfiguration.cpp) que indica la relación entre las acciones y los dispositivos.
			//También se pasa "eIA_Count" (de InputConfiguration.h) que indica cuantas acciones de entrada hay.
			cInputManager::Get().Init( kaActionMapping, eIA_Count );

			// Initialization of physics object 
			cPhysics::Get().Init();

			//Se inicializa la clase que gestiona la texturas indicando que habrá 1, por ejemplo.
			cTextureManager::Get().Init(20);

			// Terrain object
			if (!mHeightmap.Load()) OutputDebugString("Heightmap terrain load error!");

			//Se inicializa la clase que gestiona los materiales.
			cMaterialManager::Get().Init(10);

			// Init of effects management
			cEffectManager::Get().Init(10);

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

			//Se inicializa el gestor de mallas: habrá 4 mallas en la escena (mirar ./Data/Scene/) + Skeletal mesh.
			cMeshManager::Get().Init(5);

			//Se inicializa el gestor de escenas.
			cSceneManager::Get().Init(10);   

			//Se carga la escena.
			//mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/dragonsmall.DAE" ); 		
			mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/duck_triangulate.dae" ); 
			//mScene = cSceneManager::Get().LoadResource( "TestLevel", "./Data/Scene/plane.DAE" );

			// Physics object in the game
			cPhysicObject mModelObject = *((cPhysicObject*) ((cScene *)mScene.GetResource())->getSubObject( 0 ));

			cMatrix lScaleMatrix, lOffsetMatrix;

			// Preparamos ya la escala y el offset que usaran el resto de objetos
			lScaleMatrix.LoadScale( .02f );
			lOffsetMatrix.LoadTranslation( cVec3( 0.0f, 0.0f, 0.0f ) );
			// Inicializamos el modelo de esfera
			mSphereModel.InitSphere( 1.0f, 2.0f );
			for ( unsigned int luiIndex = 0; luiIndex < 10; ++luiIndex) {
				maSphereObjects.push_back( mModelObject );
				cPhysicObject &lSphereObject = maSphereObjects.back();
				cMatrix lTransMatrix;
				lTransMatrix.LoadTranslation( cVec3( 0.0, 4.0 + 5.0 * luiIndex, 0.5 * luiIndex ) ); // Las creamos formando una columna
				lSphereObject.SetWorldMatrix( lTransMatrix );
				lSphereObject.SetScaleMatrix( lScaleMatrix );
				lSphereObject.SetDrawOffsetMatrix( lOffsetMatrix );
				lSphereObject.CreatePhysics( &mSphereModel );
			}	

			cSkeletalManager::Get().Init(5);
			// Inits skeleton model
			cSkeletalManager::Get().LoadResource("Skeleton", "./Data/Skeletal/SkeletonModel.xml");
		
			// Loads skeleton 
			mSkeletalMesh = cMeshManager::Get().LoadResource("Skeleton", "Skeleton", kuiSkeletalMesh);

			// Get skeleton mesh
			cSkeletalMesh* lpSkeletonMesh=(cSkeletalMesh*)mSkeletalMesh.GetResource();

			// Play idle animation 
			lpSkeletonMesh->PlayAnim("Idle", 1.0f, 1.0f);
			
			// Load Skeleton meshes
			cResourceHandle lMaterial = cMaterialManager::Get().LoadResource("Skeleton", "./Data/Material/SkeletonMaterial.xml");
		
			assert(lMaterial.IsValidHandle());
			mObject.AddMesh(mSkeletalMesh, lMaterial);	
		
			mBoxModel.InitBox( 0.0f, cVec3( 0.3f, 1.0f, 0.3f ) );
			mObject.CreatePhysics( &mBoxModel );
			lScaleMatrix.LoadScale(0.01f);
			mObject.SetScaleMatrix( lScaleMatrix );

			cMatrix lRotation, lTranslation;
			lTranslation.LoadTranslation(cVec3(0.f, -1.f, 0.f));
			mObject.SetDrawOffsetMatrix( lTranslation );
			mObject.SetKinematic( );	

			CharacterPos::Get().Init(cVec3(0.f, 0.f, 15.f), 329.9f, 10, 0.5f);
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

	// Checks if the effect has to be reloaded
	bool lbreloadEffect = IsPressed(eIA_ReloadEffectManager);
	if (lbreloadEffect) {
		cEffectManager::Get().Reload();
	}

	bool lbmoveFront = IsPressed( eIA_MoveFront );
	bool lbmoveBack = IsPressed( eIA_MoveBack );
	bool lbmoveLeft = IsPressed( eIA_MoveLeft );
	bool lbmoveRight = IsPressed( eIA_MoveRight );
	
	cVec3 lCamaraPos = m3DCamera.GetPosition();

	// Actualiza el flag de movimiento del personaje
	CharacterPos::Get().ResetChanged();

	CharacterPos::Get().Update(lfTimestep);

	if ( lbmoveFront ) {
		CharacterPos::Get().MoveFront();
	}
	else if ( lbmoveBack ){
		CharacterPos::Get().MoveBack();
	}
	if ( lbmoveLeft ){
		CharacterPos::Get().TurnLeft();
	}
	else if ( lbmoveRight ){
		CharacterPos::Get().TurnRight();
	}

	mObject.SetPosition(CharacterPos::Get().GetCharacterPosition(), CharacterPos::Get().GetYaw());

	// Actualiza cámara y personaje
	mObject.Update(lfTimestep);
	cVec3 vVector = CharacterPos::Get().GetCharacterPosition() - CharacterPos::Get().GetFront();

	m3DCamera.SetLookAt( cVec3(vVector.x,
							   vVector.y + 0.6f,
							   vVector.z),
							   CharacterPos::Get().GetCharacterPosition(), 
							   cVec3(0.0f, 1.f, 0.f) );

	char buf[2048];
	sprintf(buf,"Camera position is (%2.2f,%2.2f, %2.2f), camera look at is (%2.0f%,%2.0f,%2.0f)\n",vVector.x,vVector.y,
			vVector.z, CharacterPos::Get().GetCharacterPosition().x, CharacterPos::Get().GetCharacterPosition().y, CharacterPos::Get().GetCharacterPosition().z);
	OutputDebugString(buf);	

	// Update bullet physics object
	cPhysics::Get().Update(lfTimestep);

	//Se actualizan los personajes:
	cCharacterManager::Get().Update(lfTimestep);

	// Update physic objects
	for ( unsigned int luiIndex = 0; luiIndex < 10; ++luiIndex) {
		maSphereObjects[luiIndex].Update(lfTimestep);
	}

	// Check if the animation keys (stop/start) are pressed
	cSkeletalMesh* lpSkeletonMesh =(cSkeletalMesh*)mSkeletalMesh.GetResource();

	static bool mbJogging = false;

	bool lbPlayJogPressed = BecomePressed(eIA_PlayJog);
	bool lbStopJogPressed = BecomePressed(eIA_StopJog);
	bool lbPlayWavePressed = BecomePressed(eIA_PlayWave);
	bool lbStopWavePressed = BecomePressed(eIA_StopWave);

	// Jog animation
	if (lbPlayJogPressed && !mbJogging){
		mbJogging = true;
		lpSkeletonMesh->PlayAnim("Jog", 1.0f, 0.1f);
		lpSkeletonMesh->StopAnim("Idle", 0.1f);
	}else if (lbStopJogPressed && mbJogging){
		mbJogging = false;
		lpSkeletonMesh->PlayAnim("Idle", 1.0f, 0.1f);
		lpSkeletonMesh->StopAnim("Jog", 0.1f);
	}

	// Wave animation
	if (lbPlayWavePressed){
		lpSkeletonMesh->PlayAnim("Wave", 1.0f, 0.1f, 0.1f);
	}else if (lbStopWavePressed){
		lpSkeletonMesh->StopAnim("Wave", 0.1f);
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
	•    Renderizado de Geometría 3D sólida and render of the skeleton mesh
	•    Render of bullet debug info
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

	//Desactivamos la textura porque aún no tenemos materiales asignados a las mallas de la escena.
//	glDisable(GL_TEXTURE_2D);
	//Para renderizar la escena llamaremos al render de la escena a través de mScene (manejador de la escena, de tipo cResourceHandle).
	//((cScene *)mScene.GetResource())->Render();
//	glEnable(GL_TEXTURE_2D);

	// 3.0) Display the terrain mesh.
	mHeightmap.Render();

	lWorld.LoadTranslation(cVec3(0.f, 0.f, 0.f));
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	// Render the debug lines
	cGraphicManager::Get().DrawGrid();
	cGraphicManager::Get().DrawAxis();

	cGraphicManager::Get().DrawPoint( cVec3(1.5f, 0.0f, 1.5f), cVec3(1.0f, 0.0f, 1.0f) );
	cGraphicManager::Get().DrawLine( cVec3(-1.5f, 0.0f, -1.5f), cVec3(-1.5f, 0.0f, 1.5f), cVec3(1.0f, 1.0f, 0.0f) );

	lWorld.LoadIdentity();	
	cGraphicManager::Get().SetWorldMatrix(lWorld);

	// 3.1) Se dibujan los personajes:
	cCharacterManager::Get().Render();


	// 3.2) Draws debug info of bullet
	cPhysics::Get().Render();	

	// Render physic objects
	for ( unsigned int luiIndex = 0; luiIndex < 10; ++luiIndex) {
		maSphereObjects[luiIndex].Render();	
	}		

	// 3.3) Render of the skeleton mesh
	// -------------------------------------------------------------
	cMatrix lRotation, lCurrPos;
	lCurrPos.LoadIdentity();
	lRotation.LoadRotation(cVec3(0.f, 1.f, 0.f), CharacterPos::Get().GetYaw());
	lCurrPos.SetPosition(CharacterPos::Get().GetCharacterPosition());

	cGraphicManager::Get().SetWorldMatrix(lRotation * lCurrPos);

	mObject.Render();

	lWorld.LoadIdentity();	
	cGraphicManager::Get().SetWorldMatrix(lWorld);
	//cSkeletalMesh* lpSkeletonMesh = (cSkeletalMesh*)mSkeletalMesh.GetResource();
	//lpSkeletonMesh->RenderSkeleton();	

	 
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

	//Deinicializamos Lua
	cLuaManager::Get().Deinit();
	//Se libera la clase que contiene la lista de personajes:
	bool lbResult =  cCharacterManager::Get().Deinit();
	//Se libera la clase que contiene la lista de comportamientos:
	lbResult = lbResult && cBehaviourManager::Get().Deinit();	

	//Se libera la clase fuente.
	mFont.Deinit();

    //Se libera el gestor de texturas.
	cTextureManager::Get().Deinit();  
	// Deinitialization of terrain
	mHeightmap.Deinit();
	// Deinitialization of physics object 
	cPhysics::Get().Deinit();
	//Se libera el InputManager:
	cInputManager::Get().Deinit();
	//Se libera OpenGL (clase cGraphicManager):
	lbResult = cGraphicManager::Get().Deinit();
	//Se libera la ventana:
	lbResult = lbResult && cWindow::Get().Deinit();
	return lbResult;	
}