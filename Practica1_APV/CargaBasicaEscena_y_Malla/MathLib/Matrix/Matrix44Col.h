#ifndef MATRIX_44_COL_H_
#define MATRIX_44_COL_H_

// cMatrix.h include this file if COLUMN_AXIS is defined
// This is like a continuation of cMatrix.h
inline cMatrix &cMatrix::LoadPerpective( float  fovy_rad, float  aspect, float  znear, float  zfar ) {
  assert( tanf( fovy_rad*0.5f ) != 0.0f );
  assert( aspect != 0.0f );
  assert( znear > 0.0f );
  assert( zfar  > znear );

  float  yScale = 1.0f / tanf( fovy_rad*0.5f );
  float  xScale = yScale / aspect;

#ifdef RIGHT_HAND
  //xScale     0          0              0
  //0        yScale       0              0
  //0          0      zf/(zn-zf)    zn*zf/(zn-zf)
  //0          0         -1              0        
  float  zScale      = zfar / (znear-zfar);
  float  zTranslate  = znear*zfar / (znear-zfar);
  float  zProjection = -1.0f;
#else
  // xScale     0          0               0
  // 0        yScale       0               0
  // 0          0       zf/(zf-zn)   -zn*zf/(zf-zn)
  // 0          0          1               0
  float  zScale      = zfar / (zfar-znear);
  float  zTranslate  = -znear*zfar / (zfar-znear);
  float  zProjection = 1.0f;
#endif

  rows[0].Set(xScale, 0.0f,   0.0f,        0.0f);
  rows[1].Set(0.0f,   yScale, 0.0f,        0.0f);
  rows[2].Set(0.0f,   0.0f,   zScale,      zTranslate);
  rows[3].Set(0.0f,   0.0f,   zProjection, 0.0f);
  return *this;
}

inline cMatrix &cMatrix::LoadOrtho( float  left, float  right, float  bottom, float  top, float  znear, float  zfar ) {
  assert( left   != right );
  assert( bottom != top );
  assert( znear > 0.0f );
  assert( zfar  > znear );

  float  xScale = 2.0f / (right-left);
  float  yScale = 2.0f / (top-bottom);
  float  xTranslate = (left+right) / (left-right);
  float  yTranslate = (top+bottom) / (bottom-top);
  float  zTranslate = znear / (znear-zfar);

#ifdef RIGHT_HAND
  //2/(r-l)      0            0           (l+r)/(l-r)
  //0            2/(t-b)      0           (t+b)/(b-t)
  //0            0            1/(zn-zf)   zn/(zn-zf)
  //0            0            0           l
  float  zScale = 1.0f / (znear-zfar);
#else
  //2/(r-l)      0            0           (l+r)/(l-r)
  //0            2/(t-b)      0           (t+b)/(b-t)
  //0            0            1/(zf-zn)   zn/(zn-zf)  
  //0            0            0           l
  float  zScale = 1.0f / (zfar-znear);
#endif

  rows[0].Set(xScale, 0.0f,   0.0f,   xTranslate);
  rows[1].Set(0.0f,   yScale, 0.0f,   yTranslate);
  rows[2].Set(0.0f,   0.0f,   zScale, zTranslate);
  rows[3].Set(0.0f,   0.0f,   0.0f,   1.0f);
  return *this;
}

inline cMatrix &cMatrix::LoadLookAt( const cVec3 &position, const cVec3 &target, const cVec3 &upWorld ) {
#ifdef RIGHT_HAND
  //zaxis = normal(Eye - At)
  cVec3 zAxis( position - target );
  zAxis.Normalize( );
#else
  //zaxis = normal(At - Eye)
  cVec3 zAxis( target - position );
  zAxis.Normalize( );
#endif

  //xaxis = normal(Cross(Up, zaxis))
  //yaxis = Cross(zaxis, xaxis)
  //    
  // xaxis.x           xaxis.y           xaxis.z          -Dot(xaxis, eye)
  // yaxis.x           yaxis.y           yaxis.z          -Dot(yaxis, eye)
  // zaxis.x           zaxis.y           zaxis.z          -Dot(zaxis, eye)  
  //   0                 0                 0              l

  cVec3 xAxis;
  Cross( xAxis, upWorld, zAxis );
  xAxis.Normalize( );

  cVec3 yAxis;
  Cross( yAxis, zAxis, xAxis );

  rows[0] = cVec4( xAxis, -Dot(xAxis, position) );
  rows[1] = cVec4( yAxis, -Dot(yAxis, position) );
  rows[2] = cVec4( zAxis, -Dot(zAxis, position) );
  rows[3].Set( 0.0f, 0.0f, 0.0f, 1.0f );
  return *this;
}

// -------------------------------------------------------
inline cVec3 cMatrix::GetPosition( ) const {
  return cVec3( rows[0][3], rows[1][3], rows[2][3] );
}

inline cVec3 cMatrix::GetFront(  ) const {
  return cVec3( rows[0][2], rows[1][2], rows[2][2] );
}

inline cVec3 cMatrix::GetUp( ) const {
  return cVec3( rows[0][1], rows[1][1], rows[2][1] );
}

// -------------------------------------------------------
inline cVec3 cMatrix::GetLeft( ) const {
#ifdef RIGHT_HAND
  return cVec3( rows[0][0], rows[1][0], rows[2][0] );
#else
  return cVec3( -rows[0][0], -rows[1][0], -rows[2][0] );
#endif
}

// -------------------------------------------------------
inline cVec3 cMatrix::GetRight( ) const {
#ifdef RIGHT_HAND
  return cVec3( -rows[0][0], -rows[1][0], -rows[2][0] );
#else
  return cVec3( rows[0][0], rows[1][0], rows[2][0] );
#endif
}

// -------------------------------------------------------
inline void cMatrix::SetPosition( const cVec3 &pos ) {
  rows[0][3] = pos.x;
  rows[1][3] = pos.y;
  rows[2][3] = pos.z;
}

// -------------------------------------------------------
inline void cMatrix::SetAxisLeftUpFront( const cVec3 &left, const cVec3 &up, const cVec3 &front ) {
#ifdef RIGHT_HAND
  rows[0] = cVec4( left, rows[0].w );
#else
  rows[0] = cVec4( -left, rows[0].w );
#endif
  rows[1] = cVec4( up, rows[1].w );
  rows[2] = cVec4( front, rows[2].w );
}

// -------------------------------------------------------
inline void cMatrix::setAxisRightUpFront( const cVec3 &right, const cVec3 &up, const cVec3 &front ) {
#ifdef RIGHT_HAND
  rows[0] = cVec4( -right, rows[0].w );
#else
  rows[0] = cVec4( right, rows[0].w );
#endif
  rows[1] = cVec4( up, rows[1].w );
  rows[2] = cVec4( front, rows[2].w );
}

// -------------------------------------------------------
//! Aplica la transformacion definida por la matriz a un punto
/*!
	\param out Vector resultante de la transformacion
	\param point Punto que se desea transformar
	\param matrix Matriz que define la transformacion
*/
inline void TransformPoint( cVec3 &out, const cMatrix &matrix, const cVec3 &point ) {
  cVec4 point4( point, 1.0f );
  TransformCoords( point4, matrix, point4 );
  out = cVec3( point4 );
}

// -------------------------------------------------------
//! Aplica la transformacion definida por la matriz a una normal (no aplica la traslacion).
/*!
	\param out Vector resultante de la transformacion
	\param normal Vector que se desea transformar
	\param matrix Matriz que define la transformacion
*/
inline void TransformNormal( cVec3 &out, const cVec3 &normal, const cMatrix &matrix ) {
  cVec4 normal4( normal, 0.0f );
  TransformCoords( normal4, matrix, normal4 );
  out = cVec3(normal4);
}

// -------------------------------------------------------
//! Aplica la transformacion definida por la matriz a una coordenada.
/*!
	\param out Vector resultante de la transformacion
	\param coords Vector que se desea transformar
	\param matrix Matriz que define la transformacion
*/
inline void TransformCoords( cVec4 &out, const cMatrix &matrix, const cVec4 &coords ) {
  cVec4 ret;
  ret.x = Dot(matrix[0], coords);
  ret.y = Dot(matrix[1], coords);
  ret.z = Dot(matrix[2], coords);
  ret.w = Dot(matrix[3], coords);
  out = ret;

  //cVec4 ret;
  //ret.x = matrix[0][0] * coords.x + matrix[0][1] * coords.y + matrix[0][2] * coords.z + matrix[0][3] * coords.w;
  //ret.y = matrix[1][0] * coords.x + matrix[1][1] * coords.y + matrix[1][2] * coords.z + matrix[1][3] * coords.w;
  //ret.z = matrix[2][0] * coords.x + matrix[2][1] * coords.y + matrix[2][2] * coords.z + matrix[2][3] * coords.w;
  //ret.w = matrix[3][0] * coords.x + matrix[3][1] * coords.y + matrix[3][2] * coords.z + matrix[3][3] * coords.w;
  //out = ret;
}

#endif // MATRIX_44_COL_H_
