#ifndef LINEAR_SPLIT_FUNCTION_H_
#define LINEAR_SPLIT_FUNCTION_H_

#include "../Vec/VecLib.h"
#include "LinearInterpolator.h"

//! Representa una funcion a trozos
/*!
	Representa una funcion a trozos. Dados una serie de puntos, se crea una funcion a trozos que se puede evaluar para todo numero flotante.
	
   f0 = Y1                      si   x  <= X1
   f1 = Lineal(Y1, Y2)          si   X1 < x <= X2
   f2 = Lineal(Y2, Y3)          si   X2 < x <= X3
   ...
   fn-1 = Lineal(Yn-1, Yn)      si Xn-1 < x <= Xn
   fn = Yn                      si Xn   < x

   NPoints es el numero de puntos que se va a usar para definir la funcion a trozos
*/
template< unsigned      NPoints >
class cLinearSplitFunction
{
  public:
//! Inicializa la clase
/*!
	 Inicializa la clase
*/
      inline void Init( void );
//! Añade un punto a la clase
/*!
	 Añade un punto a la clase para crear la funcion lineal
    \param lfX Componente X del punto a añadir
    \param lfY Componente Y del punto a añadir
*/
      inline void Add( float  lfX, float  lfY );
//! Añade un punto a la clase
/*!
	 Añade un punto a la clase para crear la funcion lineal
    \param lfPoint Punto a añadir
*/
      void Add( cVec2 lfPoint );
//! Añade un array de puntos a la clase
/*!
	 Añade un array de puntos a la clase para crear la funcion lineal
    \param lafPoints Array de puntos
    \param luiNumPoints Numero de puntos en el array
*/
      void Add( cVec2 *lafPoints, unsigned      luiNumPoints );
//! Evalua el punto en la funcion a trozos
/*!
	 Evalua el punto en la funcion a trozos
    \param lfX Punto que se desea evaluar
    \return Valor Y correspondiente al punto evaluado
*/
      float  Eval( float  lfX );
//! Evalua el punto en la funcion a trozos utilizando una evaluacion anterior
/*!
	 Evalua el punto en la funcion a trozos asumiendo que existe una cierta continuidad con el valor evaluado anteriormente. Esta funcion sera mas rapida que la anterior si el valor evaluado es muy cercano al ultimo valor evaluado. De lo contrario puede ser mas ineficiente.
    \param lfX Punto que se desea evaluar
    \return Valor Y correspondiente al punto evaluado
*/
      float  CoherentEval( float  lfX );

  private:
      unsigned         muiAdded;
      cVec2        mvPoints[ NPoints ];
      cLinearInterpolator maInterpolators[ NPoints - 1 ];
      unsigned         muiLast;
};

template< unsigned      NPoints >  inline void 
cLinearSplitFunction< NPoints >::Init( void )
{
  muiAdded = 0;
  muiLast = NPoints >> 1;
}

template< unsigned      NPoints > void 
cLinearSplitFunction< NPoints >::Add( float  lfX, float  lfY )
{
  Add( cVec2( lfX, lfY ) );
}

template< unsigned      NPoints > void 
cLinearSplitFunction< NPoints >::Add( cVec2 lfPoint )
{
  assert( muiAdded == 0 || mvPoints[ muiAdded - 1 ].x < lfPoint.x );
  assert( muiAdded < NPoints );

  mvPoints[ muiAdded ] = lfPoint;
  if ( muiAdded > 0 )
  {
      maInterpolators[ muiAdded - 1].Init( mvPoints[ muiAdded - 1 ], mvPoints[ muiAdded ] );  
  }

  ++muiAdded;
}

template< unsigned      NPoints > void 
cLinearSplitFunction< NPoints >::Add( cVec2 *lafPoints, unsigned      luiNumPoints  )
{
  assert( luiNumPoints == NPoints );
  for ( unsigned      luiIndex = 0; luiIndex < NPoints; ++luiIndex )
  {
      Add( lafPoints[ luiIndex ] );
  }
}

template< unsigned      NPoints > float  
cLinearSplitFunction< NPoints >::Eval( float  lfX )
{  
  assert( muiAdded == NPoints );

  unsigned      luiInit = 0;
  unsigned      luiEnd = NPoints - 1;
  unsigned      luiMiddle = 0;

  while ( true )
  {
      assert( luiEnd >= luiInit );
      luiMiddle = (luiEnd + luiInit) >> 1;

      if ( luiEnd == luiInit )
      {
          // Finish the search
          luiMiddle = luiInit;
          break;
      }
      else if ( luiEnd - luiInit == 1 )
      {
          // Finish the search
          luiMiddle = luiInit;
          break;
      }
      else if ( mvPoints[ luiMiddle ].x > lfX )
      {
          luiEnd = luiMiddle;
      }
      else if ( mvPoints[ luiMiddle ].x < lfX )
      {
          luiInit = luiMiddle;
      }
      else if ( mvPoints[ luiMiddle ].x == lfX )
      {
          break;
      }
  }

  if ( luiMiddle == NPoints - 1 )
  {
      --luiMiddle;
  }

  muiLast = luiMiddle;

  // Eval the interpolator
  return maInterpolators[ luiMiddle ].Eval( lfX );
}

template< unsigned      NPoints > float  
cLinearSplitFunction< NPoints >::CoherentEval( float  lfX )
{
  unsigned      luiMiddle = muiLast;

  while ( true )
  {
      if ( (luiMiddle == 0 && mvPoints[ luiMiddle ].x > lfX ) ||
           (luiMiddle == NPoints - 1 && mvPoints[ luiMiddle ].x < lfX ) ||
           (mvPoints[ luiMiddle ].x == lfX ) ||
           (luiMiddle < NPoints - 1 && mvPoints[ luiMiddle ].x <= lfX && mvPoints[ luiMiddle + 1 ].x >= lfX ) )
      {
          break;
      }
      else if ( mvPoints[ luiMiddle ].x < lfX )
      {
          ++luiMiddle;
      }
      else if ( mvPoints[ luiMiddle ].x > lfX )
      {
          --luiMiddle;
      }
  }

  if ( luiMiddle == NPoints - 1 )
  {
      --luiMiddle;
  }

  muiLast = luiMiddle;

  // Eval the interpolator
  return maInterpolators[ luiMiddle ].Eval( lfX );
}

#endif // LINEAR_SPLIT_FUNCTION_H_
