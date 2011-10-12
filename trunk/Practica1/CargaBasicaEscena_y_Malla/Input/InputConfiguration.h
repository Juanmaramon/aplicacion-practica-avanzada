/*
En este fichero se define un enum donde se declaran las posibles acciones.
Esto sirve para indicar desde el juego (clase Game) al InputManager la relación que
existe entre las acciones y los dispositivos. 
*/

#ifndef INPUT_CONFIGURATION_H
#define INPUT_CONFIGURATION_H

enum eInputActions {
    eIA_CloseApplication = 0, //Acción para cerrar la aplicación. 
    eIA_Count   //Indica cuantas acciones de entrada hay.
}; 

#endif