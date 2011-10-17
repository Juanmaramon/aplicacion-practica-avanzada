/*
Esta Clase implementa la interfaz cGenericDevice.
La clase cPad representa a un dispositivo, el Joystick/GamePad.

En la implementación de esta clase haremos uso de la librería OIS, y todas las llamadas
relativas a esta librería quedarán encapsuladas por la clase cPad.

Los dispositivos en OIS funcionan en su mayoría a través de callbacks (función que recibe como 
argumento la dirección o puntero de otra función para poder ejecutarla) y el GamePad no es una 
excepción. 
Habrá un callback para cuando se pulse un botón (función "buttonPressed") y otro 
callback para cuando el botón se suelta (función "buttonReleased"). 
También existen los siguientes callbacks:
"axisMoved", que se dispara cuando se produce un movimiento en los ejes.
"povMoved", que se dispara al moverse el punto de vista (Point Of View, POV).
"vector3Moved".

Lo que haremos será tener un array de float ("mafInput") de tamaño "ePad_Total" y 
cada vez que recibamos una llamada indicando que se ha pulsado (o soltado) un botón o que se producido cierto
movimiento, pondremos en la posición del array, correspondiente al botón o movimiento, el valor actualizado.

Esta clase es HIJA de cGenericDevice (PADRE) y por tanto heredará de ella.
Los métodos en el padre han sido declarados de tipo VIRTUAL, para que cada hijo 
los implemente a su manera.
*/

#ifndef PAD_H
#define PAD_H

#include <Windows.h>
#include "GenericDevice.h"

#include "OISInputManager.h"
#include "OISException.h"
#include "OISJoystick.h"
#include "OISEvents.h"

//Enumerado con los botones y movimientos(X,Y) del GamePad, es decir, los diferentes canales de entrada
// que tiene el mando.
//Este enumerado se usará como referencia para posicionarnos en el array "mafInput".
enum ePad
{
   // Buttons
   ePad_Button1 = 0,
   ePad_Button2,
   ePad_Button3,
   ePad_Button4,
   ePad_Button5,
   ePad_Button6,
   ePad_Button7,
   ePad_Button8,
   ePad_Button9,
   ePad_Button10,
   ePad_Button11,
   ePad_Button12,
   ePad_ButtonLast = ePad_Button12,

   // Axis (Ejes)
   ePad_AxisFirst, 
   ePad_Axis1Y = ePad_AxisFirst,
   ePad_Axis1X,
   ePad_Axis2Y,
   ePad_Axis2X,
   ePad_Axis3Y,
   ePad_Axis3X,
   ePad_AxisLast = ePad_Axis3X,  

   // POV (Point Of View, Punto de Vista)
   ePad_POVX,
   ePad_POVY,

   ePad_Total
};

class cPad : public cGenericDevice, public OIS::JoyStickListener
{
   public:
      cPad() : mbIsValid(false) { ; }
	  
	  //Inicializa el dispositivo. Puede habar varios mandos.
      void Init()                         { Init(0); }
      void Init( unsigned luiPadIndex );

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
      bool buttonPressed( const OIS::JoyStickEvent &lArg, int liButton ); //Botón pulsado.
      bool buttonReleased( const OIS::JoyStickEvent &lArg, int liButton ); //Botón soltado.
      bool axisMoved( const OIS::JoyStickEvent &lArg, int liAxis ); //Ejes movidos.
      bool povMoved( const OIS::JoyStickEvent &lArg, int liPov ); //Punto de Vista movido.
      bool vector3Moved( const OIS::JoyStickEvent &lArg, int liIndex);

   private:

	  //El puntero mpOISJoystick contiene el puntero al objeto JoyStick suministrado por OIS.	
      OIS::JoyStick* mpOISJoystick;

	  //Array de float que guardará el valor de los diferentes canales de entrada (botones, coordenadas).
	  //Sus posiciones se corresponden con el enumerado "ePad".
      float mafInput[ePad_Total];
	  
	  //Esta variable indica el índice del JoyStick/GamePad, pues puede haber varios.
      unsigned muiPadIndex;

      bool mbIsValid;
};

#endif