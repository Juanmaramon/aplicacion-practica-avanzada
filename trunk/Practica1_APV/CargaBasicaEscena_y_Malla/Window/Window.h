//Clase que representa la ventana del juego.

#ifndef Window_H
#define Window_H

#include <Windows.h>

#include "..\Utility\Singleton.h" 
#include "ApplicationProperties.h"

//Por sencillez, asumiremos que nuestro juego tiene una sóla ventana,
// por lo que la clase cWindow será un singleton.
class cWindow : public cSingleton<cWindow>
{
	friend class cSingleton<cWindow>;
	protected:
		cWindow() { ; } // Protected constructor

		HINSTANCE   mInstance; // Holds The Instance Of The Application; Manejador para acceder al objeto que 
							   //	contiene la información asociada a la ejecución de nuestra aplicación. 
		HWND        mWnd;      // Holds Our Window Handle; Manejador para acceder al objeto que representa a la ventana.
		HDC         mDC;       // Private GDI Device Context; Manejador para acceder al Contexto de Dispositivo o HDC de la ventana
							   //   y poder pintar sobre ella.	
		
		cApplicationProperties mProperties; //Este atributo será una copia de los parámetros de entrada
											// del método Init.
		bool mbCloseApplication; //Este atributo  será consultado por la clase cGame para saber si 
								 // debe cerrar el juego porque el usuario ha cerrado la ventana.

	public:
		//Esta función se encargará de crear la ventana con los parámetros que recibe.
		bool Init( cApplicationProperties &lProperties );

		//Esta función gestiona los mensajes provenientes de la ventana.
		static LRESULT CALLBACK WndProc( HWND lWnd, UINT lMsg, WPARAM lWParam, LPARAM lLParam);

		//Esta función se encargará de actualizar la ventana y atender a posibles eventos que 
		// provengan de la misma.
		void Update();	

		//Esta función sirve para que la clase cGame pueda acceder al booleano mbCloseApplication.
		inline bool GetCloseApplication() { return mbCloseApplication; }

		//Esta función accede al handle de la ventana. 
		inline HWND GetHWND() { return mWnd; }

		//OpenGL (represantado en la clase cGraphicManager) necesita acceder al contexto del dispositivo de la ventana, 
		// al número de bits por píxel y a las dimensiones de la ventana,
		// por lo que son necesarias las siguientes funciones.
		inline unsigned GetWidth()      { return mProperties.muiWidth; }
		inline unsigned GetHeight()     { return mProperties.muiHeight; }
		inline unsigned GetBpp()        { return mProperties.muiBits; }
		inline HDC &GetHDC()            { return mDC; }

		//Esta función se encargará de eliminar la ventana.
		bool Deinit();

};

#endif