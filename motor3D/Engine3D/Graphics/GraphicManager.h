//Clase que encapsulará en la medida de lo posible todas las operaciones 
// relacionadas con OpenGL.

#ifndef GraphicManager_H
#define GraphicManager_H

#include "..\Utility\Singleton.h"
#include "GLHeaders.h"
#include "Camera.h"
#include "..\Window\Window.h"
#include "..\MathLib\MathLib.h"

class cWindow;
class cGraphicManager : public cSingleton<cGraphicManager>
{
   public:
	  //Función que se encarga de la inicialización de OpenGL.
	  //Recibe la ventana sobre la que OpenGL hará el render.
      bool Init( cWindow * lpWindow );

	  //Función que se encarga de intercambiar el backbuffer con el frontbuffer.
      void SwapBuffer();

	  //Función para renderizar un punto en el espacio.
	  //El primer parámetro es la posición donde 
	  // se renderizará el punto y el segundo parámetro será el color representado por RGB 
	  // entre 0.0 y 1.0.
	  void DrawPoint( const cVec3 &lvPosition, const cVec3 &lvColor );

	  //Función para rederizar una línea.
	  void DrawLine( const cVec3 &lvPosition1, const cVec3 &lvPosition2, const cVec3 &lvColor );

	  //Función para renderizar una malla o una rejilla.
	  void DrawGrid();

	  //Función para renderizar unos ejes de coordenadas y así
	  // saber en qué dirección está cada uno (X, Y, Z).
	  void DrawAxis();

	  //Función que establece la matriz de mundo.
	  void SetWorldMatrix( const cMatrix &lMatrix );

	  //Función que establece la cámara actual.
	  void ActivateCamera( cCamera * lpCamera );
	  
	  //Función que se encarga de la liberación de OpenGL. 
      bool Deinit();

	  // Functio  to get active camera
	  cCamera * GetActiveCamera() { return mpActiveCamera; }
	
	  // Get world view proyection matrix
	  const cMatrix &GetWVPMatrix() { return mWVPMatrix; }

	  // Get world matrix
	  const cMatrix &GetWorldMatrix() { return mWorldMatrix; }

	  friend class cSingleton<cGraphicManager>;

   protected:
      cGraphicManager() { ; } // Protected constructor.

	  cWindow*    mpWindow; // Puntero a la ventana en la que se debe renderizar.
	  GLuint      mPixelFormat; // Formato de pixel.
      HGLRC       mHRC; // Handle  al contexto de renderizado de OpenGL.

	  cCamera *   mpActiveCamera; //Puntero a la cámara actual. 
	  cMatrix     mWorldMatrix; //Matriz de mundo actual.
	  cMatrix	  mWVPMatrix; // World view proyection matrix.

   private:
	   //Función que crea el contexto de renderizado.
	   bool CreateContext( cWindow * lpWindow );

	   //Función que inicializa el estado de OpenGL.
	   void InitializeGLState();

	   //Funcion que actualiza la matriz ModelView en OpenGL.
	   void RefreshWorldView();

};

#endif