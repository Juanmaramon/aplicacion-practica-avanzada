#ifndef VEC4_H_
#define VEC4_H_

#include <math.h>
#include <cassert>

#include "../MathConst.h"

struct cVec2;
struct cVec3;

//! Esta estructura representa un vector de tamaño 3
struct cVec4 {
  float  x, y, z, w;
//! Constructor por defecto
  cVec4(void): x(0),y(0),z(0),w(0) {}
  //! Constructor
  /*!
   Constructor
	\param newx Componente X del quaternion
   \param newy Componente Y del quaternion
   \param newz Componente Z del quaternion
   \param neww Componente Z del quaternion
  */
  cVec4(float  newx, float  newy, float  newz, float  neww): x(newx), y(newy), z(newz), w(neww) {}
  //! Constructor explicito
  /*!
   Constructor
	\param vec2 Vector 2
   \param lz Tercera coordenada
   \param lw Cuarta coordenada
  */
  explicit cVec4(const cVec2 &vec2, float  lz=0, float  lw=0);
  //! Constructor explicito
  /*!
   Constructor
	\param vec3 Vector 3
   \param lw Cuarta coordenada
  */
  explicit cVec4(const cVec3 &vec3, float  lw=0 );

  //! Operador de Indexacion
  /*!
   Operador de Indexacion
	\param position indice al que se quiere acceder
   \return Componente correspondiente al indice
  */
  inline float  &operator [] (unsigned      position);
    //! Operador de Indexacion
  /*!
   Operador de Indexacion
	\param position indice al que se quiere acceder
   \return Componente correspondiente al indice
  */
  inline const float  &operator [] (unsigned      position) const;
  //! Operador de Indexacion
  /*!
   Operador de Indexacion
	\param i indice al que se quiere acceder
   \return Componente correspondiente al indice
  */
  inline float        &operator() (unsigned      i);
  //! Operador de Indexacion
  /*!
   Operador de Indexacion
	\param i indice al que se quiere acceder
   \return Componente correspondiente al indice
  */
  inline const float  &operator() (unsigned      i) const;
  //! Suma dos vectores
  /*!
   Suma dos vectores
	\param other Operando
   \return Componente correspondiente al indice
  */
  inline cVec4 &operator += (const cVec4 &other);
  //! Resta dos vectores
  /*!
   Resta dos vectores
	\param other Operando
   \return Vector resultante
  */
  inline cVec4 &operator -= (const cVec4 &other);
  //! Multiplica el vector por un escalar
  /*!
   Multiplica el vector por un escalar
	\param value Operando
   \return Vector resultante
  */
  inline cVec4 &operator *= (float  value);
  //! Divide el vector por un escalar
  /*!
   Divide el vector por un escalar
	\param value Operando
   \return Vector resultante
  */
  inline cVec4 &operator /= (float  value);

  //! Setea las componentes del vector
  /*!
   Setea las componentes del vector
	\param newx Operando
   \param newy Operando
   \param newz Operando
   \param neww Operando
  */
  inline void  Set(float  newx, float  newy, float  newz, float  neww);
  //! Calcula la longitud al cuadrado del vector
  /*!
   Calcula la longitud al cuadrado del vector
   \return El valor de la longitud al cuadrado
  */
  inline float  LengthSqr(void) const;
  //! Calcula la longitud del vector
  /*!
   Calcula la longitud del vector
   \return El valor de la longitud 
  */
  inline float  Length(void) const;
  //! Calcula la Distancia al cuadrado entre los dos puntos
  /*!
   Calcula la Distancia al cuadrado entre los dos puntos
   \param p Punto al que se calculara la distancia
   \return El valor de la distancia al cuadrado
  */  
  inline float  DistanceSqrTo( const cVec4 &p ) const;
  //! Calcula la Distancia entre los dos puntos
  /*!
   Calcula la Distancia entre los dos puntos
   \param p Punto al que se calculara la distancia
   \return El valor de la distancia al cuadrado
  */
  inline float  DistanceTo( const cVec4 &p ) const;
  //! Comprueba si la longitud del vector es cero
  /*!
   Comprueba si la longitud del vector es cero
   \return Devuelve si la longitus del vector es cero
  */
  inline bool  IsZero(void) const;
  //! Normaliza el vector
  /*!
   Normaliza el vector
   \return Devuelve una referencia al vector normalizado
  */
  inline cVec4 &Normalize(void);
  //! Almacena el valor maximo por componentes
  /*!
   Almacena el valor maximo por componentes
   \param p Punto con el que se va a comparar el valor maximo
  */
  void StoreMax( const cVec4 &p );
  //! Almacena el valor minimo por componentes
  /*!
   Almacena el valor minimo por componentes
   \param p Punto con el que se va a comparar el valor minimo
  */
  void StoreMin( const cVec4 &p );
  //! Devuelve un puntero a la zona de memoria del vector
  /*!
   Devuelve un puntero a la zona de memoria del vector
   \return Puntero a la zona de memoria
  */
  inline float        *AsFloatPointer( ) { return &x; }
  //! Devuelve un puntero a la zona de memoria del vector
  /*!
   Devuelve un puntero a la zona de memoria del vector
   \return Puntero a la zona de memoria
  */
  inline const float  *AsFloatPointer( ) const { return &x; }
};

// Externs
//! Comprueba si dos vectores son iguales
  /*!
   Comprueba si dos vectores son iguales
   \param v1 Operando 1
   \param v2 Operando 2
   \return Devuelve el resultado del operador
  */
inline bool operator == (const cVec4 &v1, const cVec4 &v2);
//! Comprueba si dos vectores son distintos
  /*!
   Comprueba si dos vectores son Distintos
   \param v1 Operando 1
   \param v2 Operando 2
   \return Devuelve el resultado del operador
  */
inline bool operator != (const cVec4 &v1, const cVec4 &v2);
//! Niega un vector
/*!
Niega un vector
\param v1 Operando 1
\return Devuelve el resultado del operador
*/
inline cVec4 operator - (const cVec4 &v1);
//! Suma dos vectores
/*!
Suma dos vectores
\param v1 Operando 1
\param v2 Operando 2
\return Devuelve el resultado del operador
*/
inline cVec4 operator + (const cVec4 &v1, const cVec4 &v2);
//! Resta dos vectores
/*!
Resta dos vectores
\param v1 Operando 1
\param v2 Operando 2
\return Devuelve el resultado del operador
*/
inline cVec4 operator - (const cVec4 &v1, const cVec4 &v2);
//! Multiplica un vector por un escalar
/*!
Multiplica un vector por un escalar
\param v1 Operando 1
\param value Valor
\return Devuelve el resultado del operador
*/
inline cVec4 operator * (const cVec4 &v1, float  value);
//! Multiplica un vector por un escalar
/*!
Multiplica un vector por un escalar
\param value Valor
\param v1 Operando 1
\return Devuelve el resultado del operador
*/
inline cVec4 operator * (float  value, const cVec4 &v1);
//! Divide un vector por un escalar
/*!
Divide un vector por un escalar
\param value Valor
\param v1 Operando 1
\return Devuelve el resultado del operador
*/
inline cVec4 operator / (const cVec4 &v1, float  value);

// -------------------------------------------------------
// Operators
inline float  &cVec4::operator [] (unsigned      position) {
  assert(position<4);
  return *(&x+position);
}

inline const float  &cVec4::operator [] (unsigned      position) const {
  assert(position<4);
  return *(&x+position);
}

inline float  &cVec4::operator() (unsigned      i) {
  assert(i > 0 && i <= 4);
  return (*this)[i-1]; // Access appropriate value
}

inline const float  &cVec4::operator() (unsigned      i) const {
  assert(i > 0 && i <= 4);
  return (*this)[i-1]; // Access appropriate value
}

inline cVec4 &cVec4::operator += (const cVec4 &other) {
  x += other.x;
  y += other.y;
  z += other.z;
  w += other.w;
  return *this;
}

inline cVec4 &cVec4::operator -= (const cVec4 &other) {
  x -= other.x;
  y -= other.y;
  z -= other.z;
  w -= other.w;
  return *this;
}

inline cVec4 &cVec4::operator *= (float  value) {
  x *= value;
  y *= value;
  z *= value;
  w *= value;
  return *this;
}

inline cVec4 &cVec4::operator /= (float  value) {
  assert( value != 0.0f );
  float  inv = 1.0f / value;
  x *= inv;
  y *= inv;
  z *= inv;
  w *= inv;
  return *this;
}

// -------------------------------------------------------
// Functions
inline void  cVec4::Set(float  newx, float  newy, float  newz, float  neww) {
  x = newx;
  y = newy;
  z = newz;
  w = neww;
}

inline float  cVec4::LengthSqr(void) const {
  return (x*x + y*y + z*z + w*w);
}

inline float  cVec4::Length(void) const {
  assert( LengthSqr() >= 0.0f );
  return sqrtf( LengthSqr() );
}

inline float  cVec4::DistanceSqrTo( const cVec4 &p ) const {
  cVec4 this2p = p - (*this);
  return this2p.LengthSqr( );
}

inline float  cVec4::DistanceTo( const cVec4 &p ) const {
  cVec4 this2p = p - (*this);
  return this2p.Length( );
}

inline bool  cVec4::IsZero(void) const {
  return (fabsf(x) < EPSILON) && (fabsf(y) < EPSILON) && (fabsf(z) < EPSILON) && (fabsf(w) < EPSILON);
}

inline cVec4 &cVec4::Normalize(void) { 
  assert( Length() > EPSILON );
  (*this) /= Length();
  return *this;
}

// -------------------------------------------------------
// Externs
inline bool operator == (const cVec4 &v1, const cVec4 &v2) {
  return ((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w));
}

inline bool operator != (const cVec4 &v1, const cVec4 &v2) {
  return ((v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z) || (v1.w != v2.w));
}

inline cVec4 operator - (const cVec4 &v1) {
  return cVec4(-v1.x, -v1.y, -v1.z, -v1.w);
}

inline cVec4 operator + (const cVec4 &v1, const cVec4 &v2) {
  return cVec4( v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w );
}

inline cVec4 operator - (const cVec4 &v1, const cVec4 &v2) {
  return cVec4( v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w);
}

inline cVec4 operator * (const cVec4 &v1, float  value) {
  return cVec4( v1.x*value, v1.y*value, v1.z*value, v1.w*value );
}

inline cVec4 operator * (float  value, const cVec4 &v1) {
  return (v1*value);
}

inline cVec4 operator / (const cVec4 &v1, float  value) {
  assert( value!=0.0f );
  float  inv = 1.0f / value;
  return cVec4( v1.x*inv, v1.y*inv, v1.z*inv, v1.w*inv );
}

#endif // VEC4_H_
