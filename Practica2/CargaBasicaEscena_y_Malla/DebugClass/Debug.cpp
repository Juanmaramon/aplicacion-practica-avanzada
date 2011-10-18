#include "Debug.h"

#include <windows.h>

//Función que inicializa el mensaje que se imprimirá
bool cDebug::Init(char* lpcMsg)
{
	mpcMsg = lpcMsg;
	return true;
}

//Función que sobrecarga el operador de flujo << para volcar un mensaje en la ventana del OutPut
// en el modo Debug.
void operator<<(ostream& os, const cDebug &lDebug){
	//"OutputDebugString" vuelca la cadena a la ventana de Resultados (OutPut) de Visual Studio.
	OutputDebugString(lDebug.mpcMsg);	
}

//Función para finalizar la clase.
bool cDebug::Deinit()
{
	return true;
}
