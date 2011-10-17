#ifndef cPlane_H_
#define cPlane_H_

#include "../Vec/VecLib.h"

//! Representa un plano en el espacio
/*!
	Esta clase calcula un plano a traves de:
      - La normal del plano y un punto contenido en el mismo
      - Tres puntos contenidos en el plano
	
	f(x) = Ax + By + Cz + D

	Despues permite evaluar puntos para saber si estan contenidos en el plano o su posicion con respecto a este.
*/
class cPlane
{
  public:
//! Inicializa el plano con su normal y un punto contenido en el.
/*!
 	 Inicaliza el plano con su normal y un punto contenido en el.
    \param lvNormal Normal del plano.
    \param lvPoint Punto contenido en el plano.
*/
      inline void Init( const cVec3 &lvNormal, const cVec3 &lvPoint )
      {
          mcPlaneParams.x = lvNormal.x;
          mcPlaneParams.y = lvNormal.y;
          mcPlaneParams.z = lvNormal.z;
          mcPlaneParams.w = -Dot( lvNormal, lvPoint );
      }

//! Inicializa el plano tres puntos contenidos en el.
/*!
 	 Inicaliza el plano tres puntos contenidos en el.
    \param lvPoint1 Punto 1 contenido en el plano.
    \param lvPoint2 Punto 2 contenido en el plano.
    \param lvPoint3 Punto 3 contenido en el plano.
*/
      inline void Init( const cVec3 &lvPoint1, const cVec3 &lvPoint2, const cVec3 &lvPoint3 )
      {
          cVec3 lvNormal;
          Cross( lvNormal, lvPoint2 - lvPoint1, lvPoint3 - lvPoint1 );
          Init( lvNormal, lvPoint1 );
      }
//! Comprueba si el punto esta contenido o no en el plano
/*!
 	 Comprueba si el punto esta contenido o no en el plano.
      - Si devuelve 0, el punto esta contenido en el plano
      - Si devuelve un numero positivo, el punto esta en la direccion a la normal (delante del plano)
      - Si devuelve un numero negativo, el punto esta en la direccion opuesta a la normal (detras del plano)

    \param lvPoint1 Punto que se desea evaluar.
*/
      inline float  PointTest( const cVec3 &lvPoint1 ) const
      {
          return lvPoint1.x * mcPlaneParams.x + lvPoint1.y * mcPlaneParams.y +
                 lvPoint1.z * mcPlaneParams.z + mcPlaneParams.w;
      }

  private:
      cVec4 mcPlaneParams;
};

#endif // cPlane_H_
