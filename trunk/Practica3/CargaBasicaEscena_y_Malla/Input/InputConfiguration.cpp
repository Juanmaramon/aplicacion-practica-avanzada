#include "InputConfiguration.h"
#include "InputManager.h"
 

//Tabla (array) cuyos elementos son del tipo "tActionMapping" de InputManager.h.
//Esta tabla indica la relación entre las acciones y los dispositivos.
//El primer parámetro de cada terna es del enumerado "eInputActions" del fichero InputConfiguration.h,
// y el segundo parámetro es del enumerado "eDevices" de InputManager.h.
tActionMapping kaActionMapping[] = {
	{ eIA_CloseApplication, eKeyboard, OIS::KC_ESCAPE }, //Esta fila indica que la acción salir con el teclado está asociada a la tecla escape.	 
	{ eIA_ReloadEffectManager, eKeyboard, OIS::KC_R }, // This row associate R keyboard key with the reload action
	{ eIA_PlayJog, eKeyboard, OIS::KC_P }, // This row associate P keyboard key with the Play the Joy animation action
	{ eIA_StopJog, eKeyboard, OIS::KC_S }, // This row associate S keyboard key with the Stop the Joy animation action
	{ eIA_PlayWave, eKeyboard, OIS::KC_Q }, // This row associate Q keyboard key with the Stop the Wave animation action
	{ eIA_StopWave, eKeyboard, OIS::KC_W }, // This row associate W keyboard key with the Stop the Wave animation action
	{ eIA_MoveFront, eKeyboard, OIS::KC_UP }, // This row associate UP keyboard key with the front movement of the skeleton
	{ eIA_MoveBack, eKeyboard, OIS::KC_DOWN }, // This row associate W keyboard key with the back movement of the skeleton
	{ eIA_MoveLeft, eKeyboard, OIS::KC_LEFT }, // This row associate W keyboard key with the left movement of the skeleton
	{ eIA_MoveRight, eKeyboard, OIS::KC_RIGHT }, // This row associate W keyboard key with the right movement of the skeleton
	{ eIA_CloseApplication, ePad, ePad_Button1 }, //Esta fila indica que la acción salir con el JoyStick/GamePad está asociada al botón 1.	                                                     	 
	{ eIA_CloseApplication, eMouse, eMouse_ButtonMiddle }, //Esta fila indica que la acción salir con el mouse está asociada al botón central.	                                                     	 
    { -1, -1, -1}  // End of the table (Marca el final de la tabla)
};