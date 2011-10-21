#ifndef MATRIX_FUNC_H_
#define MATRIX_FUNC_H_

#include "Matrix.h"

//! Rota una matriz con respecto a un eje.
/*!
      \param out Matrix resultante de hacer la rotaci&oacute;n.
      \param matrix Matrix que se quiere rotar.
	  \param local_axis Eje sobre el que se quiere llevar a cabo la rotaci&oacute;n. Debe ser un vector normalizado.
	  \param rad Cantidad de grados que se quiere rotar la matriz en radianes
*/
inline void rotateLocalAxis( cMatrix &out, const cMatrix &matrix, const cVec3 &local_axis, float  rad ) {
  out = matrix;
  cVec3 pos = out.GetPosition( );
  out.SetPosition( cVec3(0,0,0) );

  cMatrix rotAxis;
  rotAxis.LoadRotation( local_axis, rad );

#ifdef ROW_AXIS
  out *= rotAxis;
#else
  out = rotAxis * out;
#endif

  out.SetPosition( pos );
}

#endif // MATRIX_FUNC_H_
