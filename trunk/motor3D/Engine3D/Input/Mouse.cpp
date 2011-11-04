#include <cassert>
#include "Mouse.h"
#include "InputManager.h"

//Método que inicializa el dispositivo.
void cMouse::Init()
{
   //Clear the input buffer.
   //Se inicializa el array de float a 0.0 en todas sus posiciones.
   memset(mafInput, 0, eMouse_Total*4);      
   
   assert(cInputManager::Get().mpOISInputManager);  
   
   //Se crea el objeto Mouse.
   mpOISMouse = (OIS::Mouse*)cInputManager::Get().mpOISInputManager->createInputObject( OIS::OISMouse, true );
   
   //Se registran los callbacks.
   mpOISMouse->setEventCallback( this );

   //Se indica el área de nuestra pantalla.
   mpOISMouse->getMouseState().height = 480;
   mpOISMouse->getMouseState().width = 640;


   mbIsValid = true;  
}

//Método que libera el dispositivo.
void cMouse::Deinit(void)
{
   assert(cInputManager::Get().mpOISInputManager);
   //Se destruye el objeto Mouse.
   cInputManager::Get().mpOISInputManager->destroyInputObject( mpOISMouse );
   //Se resetean los valores de las variables.
   mpOISMouse = 0;
   mbIsValid = false;
}

//Método que se llamará en cada frame para actualizar el estado del dispositivo.
void cMouse::Update(void)
{   
      //This fires off buffered events for Mouse
      assert(mpOISMouse);
	  //Le indicamos a la librería (OIS) que debe actualizar el estado del device (Mouse).
      mpOISMouse->capture();   
}

//Método que permitirá conocer el valor de uno de los canales de entrada (botón, ejes).
float cMouse::Check(unsigned luiEntry)
{
   if ( luiEntry < eMouse_Total )
   {
      return mafInput[luiEntry];
   }
   return 0.0f;
}

//Implementación de los callbacks:

bool cMouse::mousePressed( const OIS::MouseEvent &lArg, OIS::MouseButtonID id )
{
   if ( id <= eMouse_ButtonLast )
   {
      mafInput[id] = 1.0f;
   }
   //	std::cout << std::endl << lArg.device->vendor() << ". Button Pressed # " << liButton;
	return true;
}

bool cMouse::mouseReleased( const OIS::MouseEvent &lArg, OIS::MouseButtonID id )
{
   if ( id <= eMouse_ButtonLast )
   {
      mafInput[id] = 0.0f;
   }
   //	std::cout << std::endl << lArg.device->vendor() << ". Button Released # " << liButton;
   return true;
}

bool cMouse::mouseMoved( const OIS::MouseEvent &lArg )
{   
	mafInput[eMouse_AxisX] = (float)lArg.state.X.abs;
	mafInput[eMouse_AxisY] = (float)lArg.state.Y.abs;
	mafInput[eMouse_AxisZ] = (float)lArg.state.Z.abs;
	
	return true;	

}
