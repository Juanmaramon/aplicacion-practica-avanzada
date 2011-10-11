#include <windows.h>
#include <iostream>
#include <sstream>
 
#include "InputManager.h"
#include "../Window/Window.h"
 

//Método que inicializa el InputManager.
void cInputManager::Init( const tActionMapping laActionMapping[],
                         unsigned luiActionCount )
{
	//Inicialización de la librería OIS.    
	OIS::ParamList pl;
 
	//Esta librería (OIS), necesita el handle de la ventana, es por ello que
	// se obtiene el handle de la ventana y se le pasa a la librería. 
  
	std::ostringstream wnd;
	wnd << (size_t)cWindow::Get().GetHWND();

	pl.insert(std::make_pair( std::string("WINDOW"), wnd.str() ));

	//Default mode is foreground exclusive..but, we want to show mouse - so nonexclusive
	pl.insert(std::make_pair(std::string("w32_mouse"),
		   std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), 
		   std::string("DISCL_NONEXCLUSIVE")));

	//This never returns null.. it will raise an exception on errors
	mpOISInputManager = OIS::InputManager::createInputSystem(pl);

	//Lets enable all addons that were compiled in:
	mpOISInputManager->enableAddOnFactory(OIS::InputManager::AddOn_All);

	//Inicialización de los dispositivos: crear los objetos 
	// dispositivos y llamar a sus métodos Init.

	//Teclado.
	cGenericDevice * lpDevice = new cKeyboard;
	lpDevice->Init(); 
    
	//Se añaden los dispositivos al vector mDevices.
	//El orden de los dispositivos en este array deben coincidir 
	// con los índices asignados en el enum "eDevices":
	//   eKeyboard = 0 -> El teclado estará en el índice 0 del vector.
	//   ePad = 1      -> El JoyStick/GamePad estará en el índice 1 del vector.
    //   eMouse = 2      -> El Mouse estará en el índice 2 del vector.
	mDevices.push_back(lpDevice);       

	//JoyStick.
	lpDevice = NULL;
	lpDevice = new cPad;
	lpDevice->Init();
	mDevices.push_back(lpDevice);

	//Mouse.
	lpDevice = NULL;
	lpDevice = new cMouse;
	lpDevice->Init();
	mDevices.push_back(lpDevice);

	//Inicializar el vector de InputActions: establecer el tamaño adecuado al vector y 
	// llamar al Init de las acciones. 
	mActions.resize( luiActionCount );
	for (unsigned int i = 0; i < mActions.size(); i++)
	{
		cInputAction lAction;
        lAction.Init(); 
		mActions.at(i) = lAction;
	}

	//Crear la estructura que mantiene el mapeado de cada una de las acciones: 
	// Esta estructura se rellenará con la tabla (laActionMapping[]) que recibimos por parámetro.
	mMapped.resize( luiActionCount );
	int liIndex = 0;
	while ( laActionMapping[ liIndex ].miAction >= 0 )
	{
		cInputEntry lEntry;
		lEntry.muiDevice = laActionMapping[ liIndex ].miDevice;
		lEntry.muiChannel = laActionMapping[ liIndex ].miChannel;
		mMapped[ laActionMapping[ liIndex ].miAction ].push_back(lEntry);

		++liIndex;
	}
}

//Método que se encarga de actualizar el estado de las acciones consultando el 
// estado de los dispositivos.
void cInputManager::Update(const float &lfTimestep)
{
	 // Update the devices
    int liSize = mDevices.size();
    for (int liDeviceId = 0; liDeviceId < liSize; ++liDeviceId){
        if (mDevices[liDeviceId] != NULL)
            mDevices[liDeviceId]->Update();
    }
 
    // For each action
    liSize = mMapped.size();
    for (int lActionId = 0; lActionId < liSize; ++lActionId)
    {
        // For each map
        int liMapSize = mMapped[lActionId].size();
        float lfValue = 0.0f;
        for (int liMapId = 0; liMapId < liMapSize; ++liMapId)
        {
            // Calculate the value of the input
            const cInputEntry& lEntry = mMapped[lActionId][liMapId];
            lfValue += Check(lEntry.muiDevice, lEntry.muiChannel);
        }
 
        // Set the value in the action
        mActions[lActionId].Update(lfTimestep, lfValue);
    }
}

//Método comprueba el estado del dispositivo, es decir, si éste es válido
float cInputManager::Check(int liDevice, int liChannel) {
    if (mDevices[liDevice] != NULL && mDevices[liDevice]->IsValid() )
        return (mDevices[liDevice]->Check(liChannel));
    return 0.0f;
}

//Método que libera los recursos solicitados.
void cInputManager::Deinit()
{
   // Deinit devices
   for (unsigned luiIndex = 0; luiIndex < mDevices.size(); ++luiIndex)
   {
      mDevices[luiIndex]->Deinit();
      delete mDevices[luiIndex];
   }
   mDevices.resize(0);
   mActions.resize(0);
   mMapped.resize(0);
 
   //Para destruir la librería OIS sólo hay que hacer una llamada a destroyInputSystem.
   if( mpOISInputManager )
   {  
	  OIS::InputManager::destroyInputSystem(mpOISInputManager);
	  mpOISInputManager = NULL;
   }
}