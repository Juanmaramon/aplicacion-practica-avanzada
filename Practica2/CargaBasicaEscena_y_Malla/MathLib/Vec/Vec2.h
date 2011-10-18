#ifndef VEC2_H_
#define VEC2_H_

#include <math.h>
#include <cassert>

#include "../MathConst.h"

struct cVec3;
struct cVec4;

// -------------------------------------------------------
//! Esta estructura representa un vector de tamaño 2
struct cVec2 {
  float  x, y;

  //! Constructor por defecto
  cVec2(void): x(0),y(0) {}
  //! Constructor
  /*!
   Constructor
	\param newx Componente X del quaternion
   \param newy Componente Y del quaternion
  */
  cVec2(float  newx, float  newy): x(newx), y(newy) {}
  //! Constructor explicito
  /*!
   Constructor
	\param vec3 Vector 3
  */
  explicit cVec2(const cVec3 &vec3);
  //! Constructor explicito
  /*!
   Constructor
	\param vec4 Vector 4
  */
  explicit cVec2(const cVec4 &vec4);

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
  inline cVec2 &operator += (const cVec2 &other);
  //! Resta dos vectores
  /*!
   Resta dos vectores
	\param other Operando
   \return Vector resultante
  */
  inline cVec2 &operator -= (const cVec2 &other);
  //! Multiplica el vector por un escalar
  /*!
   Multiplica el vector por un escalar
	\param value Operando
   \return Vector resultante
  */
  inline cVec2 &operator *= (float  value);
  //! Divide el vector por un escalar
  /*!
   Divide el vector por un escalar
	\param value Operando
   \return Vector resultante
  */
  inline cVec2 &operator /= (float  value);

  //! Setea las componentes del vector
  /*!
   Setea las componentes del vector
	\param newx Operando
   \param newy Operando
  */
  inline void  Set(float  newx, float  newy);
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
  inline float  DistanceSqrTo( const cVec2 &p ) const;
  //! Calcula la Distancia entre los dos puntos
  /*!
   Calcula la Distancia entre los dos puntos
   \param p Punto al que se calculara la distancia
   \return El valor de la distancia al cuadrado
  */
  inline float  DistanceTo( const cVec2 &p ) const;
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
  inline cVec2 &Normalize(void);
  //! Almacena el valor maximo por componentes
  /*!
   Almacena el valor maximo por componentes
   \param p Punto con el que se va a comparar el valor maximo
  */
  void StoreMax( const cVec2 &p );
  //! Almacena el valor minimo por componentes
  /*!
   Almacena el valor minimo por componentes
   \param p Punto con el que se va a comparar el valor minimo
  */
  void StoreMin( const cVec2 &p );

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
inline bool operator == (const cVec2 &v1, const cVec2 &v2);
//! Comprueba si dos vectores son distintos
  /*!
   Comprueba si dos vectores son Distintos
   \param v1 Operando 1
   \param v2 Operando 2
   \return Devuelve el resultado del operador
  */
inline bool operator != (const cVec2 &v1, const cVec2 &v2);
//! Niega un vector
/*!
Niega un vector
\param v1 Operando 1
\return Devuelve el resultado del operador
*/
inline cVec2 operator - (const cVec2 &v1);
//! Suma dos vectores
/*!
Suma dos vectores
\param v1 Operando 1
\param v2 Operando 2
\return Devuelve el resultado del operador
*/
inline cVec2 operator + (const cVec2 &v1, const cVec2 &v2);
//! Resta dos vectores
/*!
Resta dos vectores
\param v1 Operando 1
\param v2 Operando 2
\return Devuelve el resultado del operador
*/
inline cVec2 operator - (const cVec2 &v1, const cVec2 &v2);
//! Multiplica un vector por un escalar
/*!
Multiplica un vector por un escalar
\param v1 Operando 1
\param value Valor
\return Devuelve el resultado del operador
*/
inline cVec2 operator * (const cVec2 &v1, float  value);
//! Multiplica un vector por un escalar
/*!
Multiplica un vector por un escalar
\param value Valor
\param v1 Operando 1
\return Devuelve el resultado del operador
*/
inline cVec2 operator * (float  value, const cVec2 &v1);
//! Divide un vector por un escalar
/*!
Divide un vector por un escalar
\param value Valor
\param v1 Operando 1
\return Devuelve el resultado del operador
*/
inline cVec2 operator / (const cVec2 &v1, float  value);

// -------------------------------------------------------
// Operators
inline float  &cVec2::operator [] (unsigned      position) {
  assert(position<2);
  return *(&x+position);
}

inline const float  &cVec2::operator [] (unsigned      position) const {
  assert(position<2);
  return *(&x+position);
}

inline float  &cVec2::operator() (unsigned      i) {
  assert(i > 0 && i <= 2);
  return (*this)[i-1]; // Access appropriate value
}

inline const float  &cVec2::operator() (unsigned      i) const {
  assert(i > 0 && i <= 2);
  return (*this)[i-1]; // Access appropriate value
}

inline cVec2 &cVec2::operator += (const cVec2 &other) {
  x += other.x;
  y += other.y;
  return *this;
}

inline cVec2 &cVec2::operator -= (const cVec2 &other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

inline cVec2 &cVec2::operator *= (float  value) {
  x *= value;
  y *= value;
  return *this;
}

inline cVec2 &cVec2::operator /= (float  value) {
  assert( value != 0.0f );
  float  inv = 1.0f / value;
  x *= inv;
  y *= inv;
  return *this;
}

// -------------------------------------------------------
// Functions
inline void  cVec2::Set(float  newx, float  newy) {
  x = newx;
  y = newy;
}

inline float  cVec2::LengthSqr(void) const {
  return (x*x + y*y);
}

inline float  cVec2::Length(void) const {
  assert( LengthSqr() >= 0.0f );
  return sqrtf( LengthSqr() );
}

inline float  cVec2::DistanceSqrTo( const cVec2 &p ) const {
  cVec2 this2p = p - (*this);
  return this2p.LengthSqr( );
}

inline float  cVec2::DistanceTo( const cVec2 &p ) const {
  cVec2 this2p = p - (*this);
  return this2p.Length( );
}

inline bool  cVec2::IsZero(void) const {
  return(fabsf(x) < EPSILON) && (fabsf(y) < EPSILON);
}

inline cVec2 &cVec2::Normalize(void) { 
  assert( Length() > EPSILON );
  (*this) /= Length();
  return *this;
}

// -------------------------------------------------------
// Externs
inline bool operator == (const cVec2 &v1, const cVec2 &v2) {
  return ((v1.x == v2.x) && (v1.y == v2.y));
}

inline bool operator != (const cVec2 &v1, const cVec2 &v2) {
  return ((v1.x != v2.x) || (v1.y != v2.y));
}

inline cVec2 operator - (const cVec2 &v1) {
  return cVec2(-v1.x, -v1.y);
}

inline cVec2 operator + (const cVec2 &v1, const cVec2 &v2) {
  return cVec2( v1.x+v2.x, v1.y+v2.y );
}

inline cVec2 operator - (const cVec2 &v1, const cVec2 &v2) {
  return cVec2( v1.x-v2.x, v1.y-v2.y );
}

inline cVec2 operator * (const cVec2 &v1, float  value) {
  return cVec2( v1.x*value, v1.y*value );
}

inline cVec2 operator * (float  value, const cVec2 &v1) {
  return (v1*value);
}

inline cVec2 operator / (const cVec2 &v1, float  value) {
  assert( value!=0.0f );
  float  inv = 1.0f / value;
  return cVec2( v1.x*inv, v1.y*inv );
}

#endif // VEC2_H_
