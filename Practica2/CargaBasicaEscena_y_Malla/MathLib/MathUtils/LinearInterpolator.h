#ifndef LINEAR_INTERPOLATOR_H_
#define LINEAR_INTERPOLATOR_H_

#include "../Vec/VecLib.h"

//! Calcula una interpolacion lineal
/*!
	Dado dos puntos, calcula la expresion de la recta que une esos dos puntos.
	
	f(x) = Ax + B

	Despues permite evaluar los valores para otros puntos que no eran los iniciales
*/
class cLinearInterpolator
{
  public:
//! Inicializa el interpolador pasandole los dos puntos (X0,Y0) y (X1,Y1)
/*!
	\param lfX0 Componente X del primer punto
	\param lfY0 Componente Y del primer punto
	\param lfX1 Componente X del segundo punto
	\param lfY1 Componente Y del segundo punto
*/
      inline void Init( float  lfX0, float  lfY0, float  lfX1, float  lfY1 )
      {
          assert( fabs( lfX1 - lfX0) > EPSILON );
          mfParamA = (lfY1 - lfY0) / ( lfX1 - lfX0 );
          mfParamB = -mfParamA * lfX0 + lfY0;
      }
//! Inicializa el interpolador pasandole los dos puntos lP0 y lP1
/*!
	\param lP0 Primer punto
	\param lP1 Segundo punto
*/
      inline void Init( const cVec2 &lP0, const cVec2 &lP1 )
      {
          Init( lP0.x, lP0.y, lP1.x, lP1.y );
      }
//! Evalua el interpolador o la recta en el punto indicado
/*!
	\param lfX Punto que se quiere evaluar
	\return Valor del interpolador o la recta en ese punto
*/
      inline float  Eval( float  lfX )
      {
          return mfParamA * lfX + mfParamB;
      }

  private:
      float  mfParamA;
      float  mfParamB;
};

#endif // LINEAR_INTERPOLATOR_H_
