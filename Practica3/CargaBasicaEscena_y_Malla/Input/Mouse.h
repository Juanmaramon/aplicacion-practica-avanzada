/*
Esta Clase implementa la interfaz cGenericDevice.
La clase cPad representa a un dispositivo, el Mouse.

En la implementación de esta clase haremos uso de la librería OIS, y todas las llamadas
relativas a esta librería quedarán encapsuladas por la clase cMouse.

Los dispositivos en OIS funcionan en su mayoría a través de callbacks (función que recibe como 
argumento la dirección o puntero de otra función para poder ejecutarla) y el Mouse no es una 
excepción. 
Habrá un callback para cuando se pulse un botón (función "mousePressed"), otro 
callback para cuando el botón se suelta (función "mouseReleased") y otro
para cuando se mueve (función "mouseMoved").

Lo que haremos será tener un array de float ("mafInput") de tamaño "kuiInputChanelSize" (8 botones + 3 componentes
 de la posición (ejes X, Y, Z) del mouse ).
Cada vez que recibamos una llamada indicando que se ha pulsado (o soltado) un botón o que se producido cierto
movimiento, pondremos en la posición del array, correspondiente al botón o movimiento, el valor actualizado.

Esta clase es HIJA de cGenericDevice (PADRE) y por tanto heredará de ella.
Los métodos en el padre han sido declarados de tipo VIRTUAL, para que cada hijo 
los implemente a su manera.
*/

#ifndef MOUSE_H
#define MOUSE_H

#include <Windows.h>
#include "GenericDevice.h"

#include "OISInputManager.h"
#include "OISException.h"
#include "OISMouse.h"
#include "OISEvents.h"


//Enumerado con los botones y movimientos(X,Y) del Mouse, es decir, los diferentes canales de entrada
// que tiene el mando.
//Este enumerado se usará como referencia para posicionarnos en el array "mafInput".
enum eMouse
{
   // Buttons
   eMouse_ButtonLeft = 0, eMouse_ButtonRight, eMouse_ButtonMiddle,
   eMouse_Button3, eMouse_Button4,	eMouse_Button5, eMouse_Button6,	eMouse_Button7,
   eMouse_ButtonLast = eMouse_Button7,

   // Axis (Ejes)
   eMouse_AxisX, eMouse_AxisY, eMouse_AxisZ,

   eMouse_Total
};


class cMouse : public cGenericDevice, public OIS::MouseListener
{
   public:
      cMouse() : mbIsValid(false) { ; }
	  
	  //Inicializa el dispositivo.
      void Init();                      

	  //Libera el dispositivo.
      void Deinit(void);

	  //Se llamará en cada frame para actualizar el estado del dispositivo.
      void Update(void);

	  //Permitirá conocer el valor de uno de los canales de entrada (botón, coordenada).
      float Check(unsigned luiEntry);

      //Indicará si el dispositivo es válido y le podemos consultar el estado de sus canales.
      inline bool IsValid(void) { return mbIsValid; }

      // Listeners for the Pad.
	  //Estas funciones son los callbacks (función que recibe como 
      // argumento la dirección o puntero de otra función para poder ejecutarla).
	  bool mouseMoved( const OIS::MouseEvent &lArg ); //Ratón movido.      
	  bool mousePressed( const OIS::MouseEvent &lArg, OIS::MouseButtonID id ); //Botón pulsado.
	  bool mouseReleased( const OIS::MouseEvent &lArg, OIS::MouseButtonID id ); //Botón soltado.	  

   private:

	  //El puntero mpOISMouse contiene el puntero al objeto Mouse suministrado por OIS.	
	  OIS::Mouse* mpOISMouse;
	   
	  //Array de float que guardará el valor de los diferentes canales de entrada (botones, coordenadas).
	  //Sus posiciones se corresponden con el enumerado "eMouse".
      float mafInput[eMouse_Total];
      
      bool mbIsValid;
};

#endif
