#ifndef MATRIX_44_ROW_H_
#define MATRIX_44_ROW_H_

// cMatrix.h include this file if ROW_AXIS is defined
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
  //0          0      zf/(zn-zf)        -1
  //0          0      zn*zf/(zn-zf)      0        
  float  zScale      = zfar / (znear-zfar);
  float  zTranslate  = znear*zfar / (znear-zfar);
  float  zProjection = -1.0f;
#else
  // xScale     0          0               0
  // 0        yScale       0               0
  // 0          0       zf/(zf-zn)         1
  // 0          0       -zn*zf/(zf-zn)     0
  float  zScale      = zfar / (zfar-znear);
  float  zTranslate  = -znear*zfar / (zfar-znear);
  float  zProjection = 1.0f;
#endif

  rows[0].Set(xScale, 0.0f,   0.0f,        0.0f);
  rows[1].Set(0.0f,   yScale, 0.0f,        0.0f);
  rows[2].Set(0.0f,   0.0f,   zScale,      zProjection);
  rows[3].Set(0.0f,   0.0f,   zTranslate,  0.0f);
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
  //2/(r-l)      0            0           0
  //0            2/(t-b)      0           0
  //0            0            1/(zn-zf)   0
  //(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l
  float  zScale = 1.0f / (znear-zfar);
#else
  //2/(r-l)      0            0           0
  //0            2/(t-b)      0           0
  //0            0            1/(zf-zn)   0
  //(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l
  float  zScale = 1.0f / (zfar-znear);
#endif

  rows[0].Set(xScale,     0.0f,       0.0f,       0.0f);
  rows[1].Set(0.0f,       yScale,     0.0f,       0.0f);
  rows[2].Set(0.0f,       0.0f,       zScale,     0.0f);
  rows[3].Set(xTranslate, yTranslate, zTranslate, 1.0f);
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
  // xaxis.x           yaxis.x           zaxis.x          0
  // xaxis.y           yaxis.y           zaxis.y          0
  // xaxis.z           yaxis.z           zaxis.z          0
  //-Dot(xaxis, eye)  -Dot(yaxis, eye)  -Dot(zaxis, eye)  l

  cVec3 xAxis;
  Cross( xAxis, upWorld, zAxis );
  xAxis.Normalize( );

  cVec3 yAxis;
  Cross( yAxis, zAxis, xAxis );

  rows[0].Set( xAxis.x,              yAxis.x,              zAxis.x,              0.0f);
  rows[1].Set( xAxis.y,              yAxis.y,              zAxis.y,              0.0f);
  rows[2].Set( xAxis.z,              yAxis.z,              zAxis.z,              0.0f);
  rows[3].Set( -Dot(xAxis,position), -Dot(yAxis,position), -Dot(zAxis,position), 1.0f);
  return *this;
}


inline cVec3 cMatrix::GetPosition( ) const {
  return cVec3( rows[3] );
}

inline cVec3 cMatrix::GetFront(  ) const {
  return cVec3( rows[2] );
}

inline cVec3 cMatrix::GetUp( ) const {
  return cVec3( rows[1] );
}

inline cVec3 cMatrix::GetLeft( ) const {
#ifdef RIGHT_HAND
  return cVec3( rows[0] );
#else
  return cVec3( -rows[0] );
#endif
}

inline cVec3 cMatrix::GetRight( ) const {
#ifdef RIGHT_HAND
  return cVec3( -rows[0] );
#else
  return cVec3( rows[0] );
#endif
}


inline void cMatrix::SetPosition( const cVec3 &pos ) {
  rows[3] = cVec4( pos, 1.0f );
}

// -------------------------------------------------------
inline void cMatrix::SetAxisLeftUpFront( const cVec3 &left, const cVec3 &up, const cVec3 &front ) {
#ifdef RIGHT_HAND
  rows[0] = cVec4( left );
#else
  rows[0] = cVec4( -left );
#endif
  rows[1] = cVec4( up );
  rows[2] = cVec4( front );
}

inline void cMatrix::setAxisRightUpFront( const cVec3 &right, const cVec3 &up, const cVec3 &front ) {
#ifdef RIGHT_HAND
  rows[0] = cVec4( -right );
#else
  rows[0] = cVec4( right );
#endif
  rows[1] = cVec4( up );
  rows[2] = cVec4( front );
}

// -------------------------------------------------------
//! Aplica la transformacion definida por la matriz a un punto
/*!
	\param out Vector resultante de la transformacion
	\param point Punto que se desea transformar
	\param matrix Matriz que define la transformacion
*/
inline void TransformPoint( cVec3 &out, const cVec3 &point, const cMatrix &matrix ) {
  cVec4 point4( point, 1.0f );
  TransformCoords( point4, point4, matrix );
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
  TransformCoords( normal4, normal4, matrix );
  out = cVec3(normal4);
}

// -------------------------------------------------------
//! Aplica la transformacion definida por la matriz a una coordenada.
/*!
	\param out Vector resultante de la transformacion
	\param coords Vector que se desea transformar
	\param matrix Matriz que define la transformacion
*/
inline void TransformCoords( cVec4 &out, const cVec4 &coords, const cMatrix &matrix ) {
  out = coords.x * matrix[0] + coords.y * matrix[1] + coords.z * matrix[2] + coords.w * matrix[3];

  //cVec4 ret;
  //ret.x = coords.x * matrix[0][0] + coords.y * matrix[1][0] + coords.z * matrix[2][0] + coords.w * matrix[3][0];
  //ret.y = coords.x * matrix[0][1] + coords.y * matrix[1][1] + coords.z * matrix[2][1] + coords.w * matrix[3][1];
  //ret.z = coords.x * matrix[0][2] + coords.y * matrix[1][2] + coords.z * matrix[2][2] + coords.w * matrix[3][2];
  //ret.w = coords.x * matrix[0][3] + coords.y * matrix[1][3] + coords.z * matrix[2][3] + coords.w * matrix[3][3];
  //out = ret;
}

#endif // MATRIX_44_ROW_H_
