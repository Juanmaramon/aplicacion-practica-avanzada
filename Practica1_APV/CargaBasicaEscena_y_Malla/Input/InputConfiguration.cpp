#include "InputConfiguration.h"
#include "InputManager.h"
 

//Tabla (array) cuyos elementos son del tipo "tActionMapping" de InputManager.h.
//Esta tabla indica la relación entre las acciones y los dispositivos.
//El primer parámetro de cada terna es del enumerado "eInputActions" del fichero InputConfiguration.h,
// y el segundo parámetro es del enumerado "eDevices" de InputManager.h.
tActionMapping kaActionMapping[] = {
	{ eIA_CloseApplication, eKeyboard, OIS::KC_ESCAPE }, //Esta fila indica que la acción salir con el teclado está asociada a la tecla escape.	                                                     	
	{ eIA_CloseApplication, ePad, ePad_Button1 }, //Esta fila indica que la acción salir con el JoyStick/GamePad está asociada al botón 1.	                                                     	 
	{ eIA_CloseApplication, eMouse, eMouse_ButtonMiddle }, //Esta fila indica que la acción salir con el mouse está asociada al botón central.	                                                     	 
    { -1, -1, -1}  // End of the table (Marca el final de la tabla)
};