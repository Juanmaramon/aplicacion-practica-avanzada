#ifndef VEC3_H_
#define VEC3_H_

#include <math.h>
#include <cassert>

#include "../MathConst.h"

struct cVec2;
struct cVec4;

//! Esta estructura representa un vector de tamaño 3
struct cVec3 {
  float  x, y, z;

  //! Constructor por defecto
  cVec3(void): x(0),y(0),z(0) {}
  //! Constructor
  /*!
   Constructor
	\param newx Componente X del quaternion
   \param newy Componente Y del quaternion
   \param newz Componente Z del quaternion
  */
  cVec3(float  newx, float  newy, float  newz): x(newx), y(newy), z(newz) {}
  //! Constructor explicito
  /*!
   Constructor
	\param vec2 Vector 2
   \param lz Tercera coordenada
  */
  explicit cVec3(const cVec2 &vec2, float  lz=0);
  //! Constructor explicito
  /*!
   Constructor
	\param vec4 Vector 4
  */
  explicit cVec3(const cVec4 &vec4);

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
  //! Operador Parentesis
  /*!
   Operador Parentesis. Los indices empiezan en 1.
	\param i indice al que se quiere acceder
   \return Componente correspondiente al indice
  */
  inline float        &operator() (unsigned      i);
  //! Operador Parentesis
  /*!
   Operador Parentesis. Los indices empiezan en 1.
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
  inline cVec3 &operator += (const cVec3 &other);
  //! Resta dos vectores
  /*!
   Resta dos vectores
	\param other Operando
   \return Vector resultante
  */
  inline cVec3 &operator -= (const cVec3 &other);
  //! Multiplica el vector por un escalar
  /*!
   Multiplica el vector por un escalar
	\param value Operando
   \return Vector resultante
  */
  inline cVec3 &operator *= (float  value);
  //! Divide el vector por un escalar
  /*!
   Divide el vector por un escalar
	\param value Operando
   \return Vector resultante
  */
  inline cVec3 &operator /= (float  value);

  //! Setea las componentes del vector
  /*!
   Setea las componentes del vector
	\param newx Operando
   \param newy Operando
   \param newz Operando
  */
  inline void  Set(float  newx, float  newy, float  newz);
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
  inline float  DistanceSqrTo( const cVec3 &p ) const;
  //! Calcula la Distancia entre los dos puntos
  /*!
   Calcula la Distancia entre los dos puntos
   \param p Punto al que se calculara la distancia
   \return El valor de la distancia al cuadrado
  */
  inline float  DistanceTo( const cVec3 &p ) const;
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
  inline cVec3 &Normalize(void);

  //! Almacena el valor maximo por componentes
  /*!
   Almacena el valor maximo por componentes
   \param p Punto con el que se va a comparar el valor maximo
  */
  void StoreMax( const cVec3 &p );
  //! Almacena el valor minimo por componentes
  /*!
   Almacena el valor minimo por componentes
   \param p Punto con el que se va a comparar el valor minimo
  */
  void StoreMin( const cVec3 &p );

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
inline bool operator == (const cVec3 &v1, const cVec3 &v2);
//! Comprueba si dos vectores son distintos
  /*!
   Comprueba si dos vectores son Distintos
   \param v1 Operando 1
   \param v2 Operando 2
   \return Devuelve el resultado del operador
  */
inline bool operator != (const cVec3 &v1, const cVec3 &v2);
//! Niega un vector
/*!
Niega un vector
\param v1 Operando 1
\return Devuelve el resultado del operador
*/
inline cVec3 operator - (const cVec3 &v1);
//! Suma dos vectores
/*!
Suma dos vectores
\param v1 Operando 1
\param v2 Operando 2
\return Devuelve el resultado del operador
*/
inline cVec3 operator + (const cVec3 &v1, const cVec3 &v2);
//! Resta dos vectores
/*!
Resta dos vectores
\param v1 Operando 1
\param v2 Operando 2
\return Devuelve el resultado del operador
*/
inline cVec3 operator - (const cVec3 &v1, const cVec3 &v2);
//! Multiplica un vector por un escalar
/*!
Multiplica un vector por un escalar
\param v1 Operando 1
\param value Valor
\return Devuelve el resultado del operador
*/
inline cVec3 operator * (const cVec3 &v1, float  value);
//! Multiplica un vector por un escalar
/*!
Multiplica un vector por un escalar
\param value Valor
\param v1 Operando 1
\return Devuelve el resultado del operador
*/
inline cVec3 operator * (float  value, const cVec3 &v1);
//! Divide un vector por un escalar
/*!
Divide un vector por un escalar
\param value Valor
\param v1 Operando 1
\return Devuelve el resultado del operador
*/
inline cVec3 operator / (const cVec3 &v1, float  value);

// -------------------------------------------------------
// Operators
inline float  &cVec3::operator [] (unsigned      position) {
  assert(position<3);
  return *(&x+position);
}

inline const float  &cVec3::operator [] (unsigned      position) const {
  assert(position<3);
  return *(&x+position);
}

inline float  &cVec3::operator() (unsigned      i) {
  assert(i > 0 && i <= 3);
  return (*this)[i-1]; // Access appropriate value
}

inline const float  &cVec3::operator() (unsigned      i) const {
  assert(i > 0 && i <= 3);
  return (*this)[i-1]; // Access appropriate value
}

inline cVec3 &cVec3::operator += (const cVec3 &other) {
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}

inline cVec3 &cVec3::operator -= (const cVec3 &other) {
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return *this;
}

inline cVec3 &cVec3::operator *= (float  value) {
  x *= value;
  y *= value;
  z *= value;
  return *this;
}

inline cVec3 &cVec3::operator /= (float  value) {
  assert( value != 0.0f );
  float  inv = 1.0f / value;
  x *= inv;
  y *= inv;
  z *= inv;
  return *this;
}

// -------------------------------------------------------
// Functions
inline void  cVec3::Set(float  newx, float  newy, float  newz) {
  x = newx;
  y = newy;
  z = newz;
}

inline float  cVec3::LengthSqr(void) const {
  return (x*x + y*y + z*z);
}

inline float  cVec3::Length(void) const {
   assert( LengthSqr() >= 0.0f );
   return sqrtf( LengthSqr() );
}

inline float  cVec3::DistanceSqrTo( const cVec3 &p ) const {
  cVec3 this2p = p - (*this);
  return this2p.LengthSqr( );
}

inline float  cVec3::DistanceTo( const cVec3 &p ) const {
  cVec3 this2p = p - (*this);
  return this2p.Length( );
}

inline bool  cVec3::IsZero(void) const {
  return (fabsf(x) < EPSILON) && (fabsf(y) < EPSILON) && (fabsf(z) < EPSILON);
}

inline cVec3 &cVec3::Normalize(void) { 
  assert(Length());
  (*this) /= Length();
  return *this;
}

// -------------------------------------------------------
// Externs
inline bool operator == (const cVec3 &v1, const cVec3 &v2) {
  return ((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z));
}

inline bool operator != (const cVec3 &v1, const cVec3 &v2) {
  return ((v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z));
}

inline cVec3 operator - (const cVec3 &v1) {
  return cVec3(-v1.x, -v1.y, -v1.z);
}

inline cVec3 operator + (const cVec3 &v1, const cVec3 &v2) {
  return cVec3( v1.x+v2.x, v1.y+v2.y, v1.z+v2.z );
}

inline cVec3 operator - (const cVec3 &v1, const cVec3 &v2) {
  return cVec3( v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

inline cVec3 operator * (const cVec3 &v1, float  value) {
  return cVec3( v1.x*value, v1.y*value, v1.z*value );
}

inline cVec3 operator * (float  value, const cVec3 &v1) {
  return (v1*value);
}

inline cVec3 operator / (const cVec3 &v1, float  value) {
  assert( value!=0.0f );
  float  inv = 1.0f / value;
  return cVec3( v1.x*inv, v1.y*inv, v1.z*inv );
}

#endif // VEC3_H_
