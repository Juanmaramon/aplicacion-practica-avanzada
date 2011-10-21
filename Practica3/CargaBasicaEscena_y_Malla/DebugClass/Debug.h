//Clase que permite mostrar mensajes por la ventana de Resultados (OutPut), sobrecargando el
// operador de flujo <<.
#ifndef Debug_H
#define Debug_H

#include <iostream>

using namespace std;

class cDebug
{
	protected:
		char * mpcMsg; //mensaje que se volcará.

	public:
		cDebug() { ; }		

		bool Init(char* lpcMsg);		
		
		//friend ostream& operator<<(ostream& os, const cDebug &lDebug);
		friend void operator<<(ostream& os, const cDebug &lDebug);

		bool Deinit();
};

#endif
