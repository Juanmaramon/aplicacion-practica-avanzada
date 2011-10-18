#ifndef ApplicationProperties_H
#define ApplicationProperties_H

#include <iostream> //Para std.

//Esta estructura será la que se pase como parámetro a la función Init de la clase cWindow.
struct cApplicationProperties
{
   // General Application Parameters
   std::string macApplicationName; //Nombre de la aplicación y de la ventana
   unsigned muiWidth; //Ancho de la Ventana
   unsigned muiHeight; //Alto de la Ventana
   unsigned muiBits; //Bits que tendrá cada píxel de la ventana. Normalmente 24 (RGB) ó 32 (RGBA) bits por píxel.
   bool mbFullscreen; //True si tiene que ejecutar a pantalla completa
};

#endif