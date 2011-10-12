//Clase que representa la cámara del juego.

#ifndef Camera_H
#define Camera_H

#include "..\MathLib\MathLib.h"

class cCamera
{
   public:
      //Función que inicializa la clase Camera.
      void Init();

	  //Para mantener la coherencia de los datos de la clase, cada vez que se modifique
	  // una de las dos matrices (vista o proyección) se deberá actualizar la matriz mViewProj,
	  // usando esta función que multiplica nView * mProj.
	  inline void RefreshViewProj()   { mViewProj = mView * mProj; }

	  //Función que define la matriz de vista. 
	  //Recibe 3 argumentos, que son:
	  //    Dónde está la cámara.
	  //    Punto al que mira la cámara.
	  //    Un  vector   que  indica  la  dirección   Up  de  la  cámara  antes  de  posicionarla  y 
	  //     orientarla con los otros dos parámetros. Normalmente este valor apunta hacia 
	  //     arriba (0,1,0).
	  void SetLookAt( const cVec3 &lvPosition, 
                         const cVec3 &lvTarget, 
                         const cVec3 &lvUpWorld );

	  //Función para definir la matriz de proyección en ortogonal (objetos lejanos y cercanos son
	  // del mismo tamaño. Este tipo de proyección es muy útil para renderizar texto e interfaces de usuario)
	  //Los primeros cuatro parámetros definen el tamaño del área que se va a renderizar, 
	  // mientras que los otros 2 definen el clipping con respecto a la distancia a la cámara.  Es 
	  // decir, que lo que la cámara mostrará por pantalla será todo aquello que se encuentre a 
	  // una distancia mayor que el lfZNear y menor que el lfZFar. El valor lfZNear siempre 
	  // tiene que ser mayor que 0.
	  void SetOrtho( float  lfLeft, float  lfRight, 
					  float  lfBottom, float  lfTop, 
					  float  lfZNear, float  lfZNFar );

	  //Función para definir la matriz de proyección en perspectiva (objetos lejanos se muestran más pequeños).
	  //La matriz de proyección perspectiva es la que normalmente se usa para renderizar el 
	  // mundo en 3D.
	  //Los parámetros son el ángulo de apertura de la cámara en el eje Y, la relación de 
	  // aspecto entre el ancho y el alto y los valores lfZNear y lfZFar que tienen el mismo 
	  // significado que en la proyección ortogonal.
	  void SetPerspective( float  lfFOVY_Rad, float  lfAspect,
							float  lfZNear, float  lfZFar );

	  //Funciones que devuelven información relativa a la orientación y posición de la cámara.
	  cVec3 GetFront()       const   { mView.GetFront(); }
	  cVec3 GetLeft()        const   { mView.GetLeft();  }
	  cVec3 GetRight()       const   { mView.GetRight(); }
	  cVec3 GetUp()          const   { mView.GetUp();    }
	  cVec3 GetPosition()    const   { mView.GetPosition();    }
 
	  //Funciones que devuelven las matrices de esta clase (camera).
	  const cMatrix &GetView() const { return mView; }
	  const cMatrix &GetProj() const { return mProj; }
	  const cMatrix &GetViewProj() const { return mViewProj; }	


   private:

	//La cámara se define a través de dos matrices: La matriz de vista (view) y la matriz de 
	// proyección (projection). La matriz de vista determina dónde está la cámara en nuestro 
	// juego y que orientación tiene. La matriz de proyección determina las características de 
	// la “lente” de la cámara y como se realizará la proyección del mundo 3D sobre el plano 
	// de la 2D de la pantalla.
      cMatrix mView;
      cMatrix mProj;
	  //Se añade una matriz adicional llamada mViewProj que contendrá la multiplicación 
	  // entre la matriz de vista y la de proyección.
      cMatrix mViewProj;
};

#endif