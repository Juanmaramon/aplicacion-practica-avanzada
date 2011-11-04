#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "../Vec/VecLib.h"
#include "../Matrix/MatrixLib.h"

//! Esta estructura representa un quaternion
struct cQuaternion
{
  float  x,y,z,w;

  //! Constructor por defecto. NO inicializa el Quaternion a la identidad.
  cQuaternion(void) : x(0),y(0),z(0),w(0) {}
  //! Constructor
  /*!
   Constructor
	\param newx Componente X del quaternion
   \param newy Componente Y del quaternion
   \param newz Componente Z del quaternion
   \param neww Componente W del quaternion
  */
  cQuaternion(float  newx,float  newy,float  newz,float  neww) : x(newx),y(newy),z(newz),w(neww) {}

  //! Compara si dos quaterniones son iguales
  /*!
   Compara si dos quaterniones son iguales
	\param other Quaternion con el que se compara
  */
  inline bool        operator == (const cQuaternion &other) const;
  //! Compara si dos quaterniones son distintos
  /*!
   Compara si dos quaterniones son distintos
	\param other Quaternion con el que se compara
  */
  inline bool        operator != (const cQuaternion &other) const;
  //! Suma dos Quaterniones
  /*!
   Suma dos Quaterniones
	\param other Quaternion con el que se opera
  */
  inline cQuaternion &operator += (const cQuaternion &other);
  //! Resta dos Quaterniones
  /*!
   Resta dos Quaterniones
	\param other Quaternion con el que se opera
  */
  inline cQuaternion &operator -= (const cQuaternion &other);
  //! Multiplica dos Quaterniones
  /*!
   Multiplica dos Quaterniones
	\param other Quaternion con el que se opera
  */
  inline cQuaternion &operator *= (const cQuaternion &other);
  //! Divide las componentes del quaternion por un valor
  /*!
   Divide las componentes del quaternion por un valor
	\param scalar Valor por el que se dividira el quaternion
  */
  inline cQuaternion &operator /= (float  scalar);
  //! Multiplica las componentes del quaternion por un valor
  /*!
   Multiplica las componentes del quaternion por un valor
	\param scalar Valor por el que se multiplicara el quaternion
  */
  inline cQuaternion &operator *= (float  scalar);

  //! Carga la identidad en el quaternion
  /*!
   Carga la identidad en el quaternion
   \return Devuelve una referencia al quaternion
  */
  inline cQuaternion &LoadIdentity( );
  //! Crea un quaternion usando angulos de Euler
  /*!
   Crea un quaternion usando angulos de Euler
	\param yaw angulo Yaw
   \param pitch angulo Pitch
   \param roll angulo Roll
   \return Devuelve una referencia al quaternion
  */
  inline cQuaternion &LoadRotationEuler( float  yaw, float  pitch, float  roll );
  //! Crea un quaternion usando notacion Eje y Rotacion
  /*!
   Crea un quaternion usando notacion Eje y Rotacion
	\param axis Eje de rotacion
   \param angle_rad angulo de rotacion en radianes
   \return Devuelve una referencia al quaternion
  */
  inline cQuaternion &LoadRotationAxis( const cVec3 &axis, float  angle_rad );
  //! Permite cambiar las componentes del quaternion
  /*!
   Permite cambiar las componentes del quaternion
	\param newx Componente X del quaternion
   \param newy Componente Y del quaternion
   \param newz Componente Z del quaternion
   \param neww Componente W del quaternion
   \return Devuelve una referencia al quaternion
  */
  inline cQuaternion &Set(float  newx,float  newy,float  newz,float  neww);
  //! Normaliza el quaternion
  /*!
   Normaliza el quaternion
   \return Devuelve una referencia al quaternion
  */
  inline cQuaternion &Normalize(void);
  //! Crea el quaternion conjugado del quaternion
  /*!
   Crea el quaternion conjugado del quaternion
   \return Devuelve una referencia al quaternion
  */
  inline cQuaternion &Conjugate();
  //! Crea el quaternion inverso del quaternion
  /*!
   Crea el quaternion inverso del quaternion
   \return Devuelve una referencia al quaternion
  */
  inline cQuaternion &Invert();
  //! Calcula la longitud al cuadrado del quaternion
  /*!
   Calcula la longitud al cuadrado del quaternion
   \return Devuelve una referencia al quaternion
  */
  inline float       LengthSqr(void) const;
  //! Calcula la longitud del quaternion
  /*!
   Calcula la longitud del quaternion
   \return Devuelve una referencia al quaternion
  */
  inline float       Length(void) const;

  //! Devuelve la matriz de rotacion equivalente al quaternion
  /*!
   \param matrix Devuelve la matriz de rotacion equivalente al quaternion
  */
  inline void AsMatrix( cMatrix &matrix ) const;
  //! Devuelve el eje y angulo equivalente al quaternion
  /*!
   Devuelve el eje y angulo equivalente al quaternion
   \param axis Devuelve el eje de rotacion equivalente al quaternion
   \param angle Devuelve el angulo de rotacion equivalente al quaternion (en radianes).
  */
  inline void AsAxis( cVec3 &axis, float  &angle ) const;
  //! Devuelve los angulos de euler equivalentes al quaternion
  /*!
   Devuelve los angulos de euler equivalentes al quaternion
   \param yaw Devuelve el angulo yaw equivalente al quaternion
   \param pitch Devuelve el angulo pitch equivalente al quaternion (en radianes).
   \param roll Devuelve el angulo roll equivalente al quaternion (en radianes).
  */
  inline void AsEuler( float  &yaw, float  &pitch, float  &roll ) const;
  //! Devuelve el quaternion como un array de flotantes
  /*!
   Devuelve el quaternion como un array de flotantes
   \return Devuelve un puntero a la memoria utilizada por el quaternion
  */
  inline float  *AsFloatPointer( ) { return &x; }
  //! Devuelve el quaternion como un array de flotantes
  /*!
   Devuelve el quaternion como un array de flotantes
   \return Devuelve un puntero a la memoria utilizada por el quaternion
  */
  inline const float  *AsFloatPointer( ) const { return &x; }
};

//! Suma dos quaterniones
/*!
   Suma dos quaterniones
   \param first Primer Operando
   \param second Segundo Operando
   \return Devuelve el quaternion resultante
*/
inline cQuaternion operator + (const cQuaternion &first, const cQuaternion &second);
//! Resta dos quaterniones
/*!
   Resta dos quaterniones
   \param first Primer Operando
   \param second Segundo Operando
   \return Devuelve el quaternion resultante
*/
inline cQuaternion operator - (const cQuaternion &first, const cQuaternion &second);
//! Niega un quaternion
/*!
   Niega un quaternion
   \param quat Operando
   \return Devuelve el quaternion resultante
*/
inline cQuaternion operator - (const cQuaternion &quat);
//! Multiplica dos quaterniones
/*!
   Multiplica dos quaterniones
   \param first Primer Operando
   \param second Segundo Operando
   \return Devuelve el quaternion resultante
*/
inline cQuaternion operator * (const cQuaternion &first, const cQuaternion &second);
//! Multiplica un quaternion por un escalar
/*!
   Multiplica un quaternion por un escalar
   \param quat Quaternion
   \param scalar Escalar
   \return Devuelve el quaternion resultante
*/
inline cQuaternion operator * (const cQuaternion &quat,float  scalar);
//! Multiplica un quaternion por un escalar
/*!
   Multiplica un quaternion por un escalar
   \param scalar Escalar
   \param quat Quaternion
   \return Devuelve el quaternion resultante
*/
inline cQuaternion operator * (float  scalar,const cQuaternion &quat);
//! Divide un quaternion por un escalar
/*!
   Divide un quaternion por un escalar
   \param quat Quaternion
   \param scalar Escalar
   \return Devuelve el quaternion resultante
*/
inline cQuaternion operator / (const cQuaternion &quat,float  scalar);

// -------------------------------------------------------
// Operators
inline bool cQuaternion::operator == (const cQuaternion &other) const  {
  return ((x==other.x) && (y==other.y) && (z==other.z) && (w==other.w));
}

inline bool cQuaternion::operator != (const cQuaternion &other) const  {
  return ((x!=other.x) || (y!=other.y) || (z!=other.z) || (w!=other.w));
}

inline cQuaternion &cQuaternion::operator += (const cQuaternion &other) {
  x+=other.x;
  y+=other.y;
  z+=other.z;
  w+=other.w;
  return *this;
}

inline cQuaternion &cQuaternion::operator -= (const cQuaternion &other) {
  x-=other.x;
  y-=other.y;
  z-=other.z;
  w-=other.w;
  return *this;
}

inline cQuaternion &cQuaternion::operator *= (const cQuaternion &other) {
  Set(w * other.x + x * other.w + y * other.z - z * other.y,
      w * other.y + y * other.w + z * other.x - x * other.z,
      w * other.z + z * other.w + x * other.y - y * other.x,
      w * other.w - x * other.x - y * other.y - z * other.z);
  return *this;
}

inline cQuaternion &cQuaternion::operator /= (float  scalar) {
  assert( scalar != 0 );
  float  inv = 1.0f / scalar;
  x*=inv;
  y*=inv;
  z*=inv;
  w*=inv;
  return *this;
}

inline cQuaternion &cQuaternion::operator *= (float  scalar) {
  x*=scalar;
  y*=scalar;
  z*=scalar;
  w*=scalar;
  return *this;
}

// -------------------------------------------------------
// Functions
inline cQuaternion &cQuaternion::LoadIdentity( ) {
  Set( 0, 0, 0, 1 );
  return *this;
}

inline cQuaternion &cQuaternion::LoadRotationEuler(float  yaw, float  pitch, float  roll) {
  float  cosy = cosf(yaw   * 0.5f);
  float  siny = sinf(yaw   * 0.5f);
  float  cosP = cosf(pitch * 0.5f);
  float  sinP = sinf(pitch * 0.5f);
  float  cosR = cosf(roll  * 0.5f);
  float  sinR = sinf(roll  * 0.5f);

  x = cosR * sinP * cosy + sinR * cosP * siny;
  y = cosR * cosP * siny - sinR * sinP * cosy;
  z = sinR * cosP * cosy - cosR * sinP * siny;
  w = cosR * cosP * cosy + sinR * sinP * siny;
  return *this;
}

inline cQuaternion &cQuaternion::LoadRotationAxis( const cVec3 &axis, float  angle_rad) {
  float  s = sinf(angle_rad * 0.5f);
  Set( axis.x * s, axis.y * s, axis.z * s, cosf(angle_rad * 0.5f) );
  return *this;
}

inline cQuaternion &cQuaternion::Set(float  newx,float  newy,float  newz,float  neww) {
  x=newx;
  y=newy;
  z=newz;
  w=neww;
  return *this;
}

inline cQuaternion &cQuaternion::Normalize(void) {
   assert( Length( ) > EPSILON );
  (*this) /= Length( );
  return *this;
}

inline cQuaternion &cQuaternion::Conjugate() {
  x=-x;
  y=-y;
  z=-z;
  return *this;
}

inline cQuaternion &cQuaternion::Invert() {
  Conjugate();
  assert( LengthSqr( ) > EPSILON );
  (*this) /= LengthSqr();
  return *this;
}

inline float  cQuaternion::LengthSqr(void) const {
  return w * w + x * x + y * y + z * z;
}

inline float  cQuaternion::Length(void) const {
   assert(LengthSqr() >= 0);
  return sqrtf( LengthSqr() );
}

inline void cQuaternion::AsMatrix( cMatrix &matrix ) const {
#ifdef ROW_AXIS
  matrix[0].Set( 1 - 2*(y*y) - 2*(z*z), 2*(x*y+w*z),           2*(x*z-w*y),           0 );
  matrix[1].Set( 2*(x*y-w*z),           1 - 2*(x*x) - 2*(z*z), 2*(y*z+w*x),           0 );
  matrix[2].Set( 2*(x*z+w*y),           2*(y*z-w*x),           1 - 2*(x*x) - 2*(y*y), 0 );
  matrix[3].Set( 0,                     0,                     0,                     1 );
#else
  matrix[0].Set( 1 - 2*(y*y) - 2*(z*z), 2*(x*y-w*z),           2*(x*z+w*y),           0 );
  matrix[1].Set( 2*(x*y+w*z),           1 - 2*(x*x) - 2*(z*z), 2*(y*z-w*x),           0 );
  matrix[2].Set( 2*(x*z-w*y),           2*(y*z+w*x),           1 - 2*(x*x) - 2*(y*y), 0 );
  matrix[3].Set( 0,                     0,                     0,                     1 );
#endif
}

inline void cQuaternion::AsAxis( cVec3 &axis, float  &angle ) const {
  angle = acosf(w);

  // pre-compute to save time
  float  sinf_theta_inv = sinf(angle);
  assert( sinf_theta_inv != 0.0f );
  sinf_theta_inv = 1.0f / sinf_theta_inv;

  // now the vector
  axis.x = x*sinf_theta_inv;
  axis.y = y*sinf_theta_inv;
  axis.z = z*sinf_theta_inv;

  // multiply by 2
  angle*=2.0f;
}

inline void cQuaternion::AsEuler( float  &yaw, float  &pitch, float  &roll ) const {
  float  sqw = w*w;
  float  sqx = x*x;
  float  sqy = y*y;
  float  sqz = z*z;

  yaw   = atan2f( 2.0f * (x*y + z*w), sqx - sqy - sqz + sqw );    		
  pitch = asinf( -2.0f * (x*z - y*w) );
  roll  = atan2f( 2.0f * (y*z + x*w), -sqx - sqy + sqz + sqw );    
}

// -------------------------------------------------------
// Externs
inline cQuaternion operator + (const cQuaternion &first, const cQuaternion &second) {
  return cQuaternion(first.x+second.x,first.y+second.y,first.z+second.z,first.w+second.w);
}

inline cQuaternion operator - (const cQuaternion &first, const cQuaternion &second) {
  return cQuaternion(first.x-second.x,first.y-second.y,first.z-second.z,first.w-second.w);
}

inline cQuaternion operator - (const cQuaternion &quat) {
  return cQuaternion(-quat.x, -quat.y, -quat.z, -quat.w);
}

inline cQuaternion operator * (const cQuaternion &first, const cQuaternion &second) {
  cQuaternion q(first);
  q *= second;
  return q;
}

inline cQuaternion operator * (const cQuaternion &quat,float  scalar) {
  return cQuaternion(quat.x*scalar,quat.y*scalar,quat.z*scalar,quat.w*scalar);
}

inline cQuaternion operator * (float  scalar,const cQuaternion &quat) {
  return (quat*scalar);
}

inline cQuaternion operator / (const cQuaternion &quat,float  scalar) {
  assert( scalar != 0 );
  float  inv = 1.0f / scalar;
  return cQuaternion(quat.x*inv,quat.y*inv,quat.z*inv,quat.w*inv);
}

#endif // QUATERNION_H_
