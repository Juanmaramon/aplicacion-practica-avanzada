/*
Esta clase representa el gestor de entrada. Aúna y coordina todos los componentes.
Se encarga de actualizar y leer los dispositivos, y con la información que obtiene de ellos 
actualizar las acciones correspondientes.
*/

#ifndef INPUT_MANAGER
#define INPUT_MANAGER
 
#include <vector>
#include "InputAction.h"
#include "InputEntry.h"
#include "Keyboard.h"
#include "Pad.h"
#include "Mouse.h"
#include "..\Utility\Singleton.h"
 
#define IsPressed( ACTION ) cInputManager::Get().GetAction( ACTION ).GetIsPressed();
 
class OIS::InputManager;
 
//Enumerado que identifica los dispositivos de entrada.
enum eDevices
{
    eKeyboard = 0, //Teclado
	ePad,          //JoyStick/GamePad
	eMouse         //Ratón 
};

//Esta estructura representa una fila de la tabla ("kaActionMapping" de InputConfiguration.cpp)
// que se utilizará para inicializar el InputManager.
//El primer atributo (miAction) es un identificador de la acción, el segundo (miDevice) 
// es un identificador del dispositivo al que se va a mapear la acción y el tercero (miChannel)
// es el canal dentro del dispositivo.
struct tActionMapping
{
    int miAction;
    int miDevice;
    int miChannel;
};
 
//Esta clase usará el patrón Singleton definido en "..Utility/Singleton.h"
class cInputManager : public cSingleton<cInputManager>
{
   public:

      //Inicializa la clase InputManager.
      void Init( const tActionMapping laActionMapping[], unsigned luiActionCount );

	  //Actualiza la clase InputManager.
      void Update(const float &lfTimestep);

	  //Finaliza la clase InputManager.
      void Deinit();
 
      inline cInputAction &GetAction(const int &lActionId) { return mActions[lActionId]; }
 
      friend class cSingleton<cInputManager>;
 
   protected:
      cInputManager() { ; } // Protected constructor
 
   private:

    //Comprueba el estado del dispositivo, es decir, si éste es válido
    float Check(int liDevice, int liChannel);
 
	//Este vector se encargará de mantener el estado de cada una de las acciones. 
	//La acción que está en la posición 0 del vector se corresponde con la acción 
	// con identificador 0 (en el enum "eInputActions"), la que está en la posición 1, 
	// con la del identificador 1, y así sucesivamente. 
    std::vector <cInputAction> mActions;

	//Array con los dispositivos que existen en el juego. 
	//El orden de los dispositivos en este array deben coincidir con los índices asignados 
	// en el enum "eDevices".
    std::vector <cGenericDevice *> mDevices;

	//Estructura que mantiene el mapeado de cada una de las acciones (una acción 
    // puede estar mapeada en varios dispositivos).
	//Esta estructura es un vector de vectores del tipo cInputEntry. 
	//Por ejemplo, supongamos que tenemos una acción con identificador 
	// 6, pues mMapped[6] devolverá el vector con cada uno de los mapeos de la 
	// acción con índice 6.  
    std::vector < std::vector < cInputEntry > > mMapped;
 
    // Specific OIS vars
    friend class cKeyboard;
	friend class cPad;
	friend class cMouse;
	//Puntero al sistema de entrada de la librería OIS.
    OIS::InputManager * mpOISInputManager;
};
 
#endif