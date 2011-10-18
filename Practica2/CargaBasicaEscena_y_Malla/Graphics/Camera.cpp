
#include "Camera.h"

//Función que inicializa la clase Camera.
void cCamera::Init()
{
	//Se inicializan las 3 matrices a la identidad:
	mView.LoadIdentity();
	mProj.LoadIdentity();
	mViewProj.LoadIdentity();
}

//Función que define la matriz de vista. 
//Recibe 3 argumentos, que son:
//    Dónde está la cámara.
//    Punto al que mira la cámara.
//    Un  vector   que  indica  la  dirección   Up  de  la  cámara  antes  de  posicionarla  y 
//     orientarla con los otros dos parámetros. Normalmente este valor apunta hacia 
//     arriba (0,1,0).
void cCamera::SetLookAt( const cVec3 &lvPosition, 
                         const cVec3 &lvTarget, 
                         const cVec3 &lvUpWorld )
{
   mView.LoadLookAt(lvPosition, lvTarget, lvUpWorld);
   RefreshViewProj();
}


//Función para definir la matriz de proyección en ortogonal (objetos lejanos y cercanos son
// del mismo tamaño. Este tipo de proyección es muy útil para renderizar texto e interfaces de usuario)
//Los primeros cuatro parámetros definen el tamaño del área que se va a renderizar, 
// mientras que los otros 2 definen el clipping con respecto a la distancia a la cámara.  Es 
// decir, que lo que la cámara mostrará por pantalla será todo aquello que se encuentre a 
// una distancia mayor que el lfZNear y menor que el lfZFar. El valor lfZNear siempre 
// tiene que ser mayor que 0.
void cCamera::SetOrtho( float  lfLeft, float  lfRight, 
						float  lfBottom, float  lfTop, 
						float  lfZNear, float  lfZNFar )
{
   mProj.LoadOrtho(lfLeft, lfRight, lfBottom, lfTop, lfZNear, lfZNFar);
   RefreshViewProj();
}

//Función para definir la matriz de proyección en perspectiva (objetos lejanos se muestran más pequeños).
//La matriz de proyección perspectiva es la que normalmente se usa para renderizar el 
// mundo en 3D.
//Los parámetros son el ángulo de apertura de la cámara en el eje Y, la relación de 
// aspecto entre el ancho y el alto y los valores lfZNear y lfZFar que tienen el mismo 
// significado que en la proyección ortogonal.
void cCamera::SetPerspective( float  lfFOVY_Rad, float  lfAspect,
								float  lfZNear, float  lfZFar )
{
   mProj.LoadPerpective(lfFOVY_Rad, lfAspect, lfZNear, lfZFar);
   RefreshViewProj();
}