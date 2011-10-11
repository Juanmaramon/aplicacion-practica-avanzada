#ifndef MATRIX_44_H_
#define MATRIX_44_H_

#include "../Vec/VecLib.h"

// -------------------------------------------------------
//! Left,  Up, Front y Posicion en Filas
#define RIGHT_HAND_ROW_AXIS       (1)  
//! Left,  Up, Front y Posicion en Columnas
#define RIGHT_HAND_COLUMN_AXIS    (2)  
//! Right, Up, Front y Posicion en Filas
#define LEFT_HAND_ROW_AXIS        (3)  
//! Right, Up, Front y Posicion en Columnas
#define LEFT_HAND_COLUMN_AXIS     (4)  
//! Configuracion actual de matriz
/*!
	Puede tener uno de los siguientes valores RIGHT_HAND_ROW_AXIS, RIGHT_HAND_COLUMN_AXIS, LEFT_HAND_ROW_AXIS, LEFT_HAND_COLUMN_AXIS
	\sa RIGHT_HAND_ROW_AXIS RIGHT_HAND_COLUMN_AXIS LEFT_HAND_ROW_AXIS LEFT_HAND_COLUMN_AXIS
*/
#define MATRIX_CONFIGURATION      RIGHT_HAND_ROW_AXIS

// -------------------------------------------------------
#if !defined(MATRIX_CONFIGURATION)
#error Must define a config for Matrices.
#endif

#if   ( MATRIX_CONFIGURATION == RIGHT_HAND_ROW_AXIS )
#define RIGHT_HAND
#define ROW_AXIS
#elif ( MATRIX_CONFIGURATION == RIGHT_HAND_COLUMN_AXIS )
#define RIGHT_HAND
#define COLUMN_AXIS
#elif ( MATRIX_CONFIGURATION == LEFT_HAND_ROW_AXIS )
#define ROW_AXIS
#elif ( MATRIX_CONFIGURATION == LEFT_HAND_COLUMN_AXIS )
#define COLUMN_AXIS
#else
#error Wrong matrix configuration.
#endif
// -------------------------------------------------------
//! Esta estructura representa una matriz de transformacion 4x4
struct cMatrix {
  cVec4 rows[4];

  //! Constructor por defecto. NO inicializa la matrix a la identidad.
  cMatrix(void) {}
  //! Constructor. Inicializa toda la matriz al valor pasado
  /*!
	\param value Valor al que se va a inicializar la matriz
  */
  cMatrix(float  value);
  //! Constructor. Inicializa la matriz con los vectores que se le pasa
  /*!
	\param v0 Vector 0 de la matriz. Consultar MATRIX_CONFIGURATION para saber a que corresponde.
	\param v1 Vector 1 de la matriz. Consultar MATRIX_CONFIGURATION para saber a que corresponde.
	\param v2 Vector 2 de la matriz. Consultar MATRIX_CONFIGURATION para saber a que corresponde.
	\param v3 Vector 3 de la matriz. Consultar MATRIX_CONFIGURATION para saber a que corresponde.
	\sa MATRIX_CONFIGURATION
  */
  cMatrix(const cVec4 &v0,const cVec4 &v1,const cVec4 &v2, const cVec4 &v3);

  // Operators
  //! Permite acceder a los vectores que componen la matriz
  /*!
	\param index Indice del vector al que se quiere acceder
	\return Devuelve el vector solicitado
  */
  inline cVec4        &operator [] (unsigned      index);
  //! Permite acceder a los vectores que componen la matriz
  /*!
	\param index indice del vector al que se quiere acceder. Los indices empiezan en 0.
	\return Devuelve el vector solicitado
  */
  inline const cVec4  &operator [] (unsigned      index) const;
  //! Permite acceder al elemento (i,j) de la matriz
  /*!
	\param i indice i al que se desea acceder. Los indices empiezan en 1.
	\param j indice j al que se desea acceder. Los indices empiezan en 1.
	\return Devuelve el valor solicitado
  */
  inline float        &operator () (unsigned      i, unsigned      j = 1);
  //! Permite acceder al elemento (i,j) de la matriz
  /*!
	\param i indice i al que se desea acceder. Los indices empiezan en 1.
	\param j indice j al que se desea acceder. Los indices empiezan en 1.
	\return Devuelve el valor solicitado
  */
  inline const float  &operator () (unsigned      i, unsigned      j = 1) const;
  //! Suma casilla a casilla todos los componentes de la matriz y lo guarda en la matriz actual
  /*!
	\param other Matriz con la que se opera.
	\return Devuelve una referencia a la matriz actual.
  */
  inline cMatrix    &operator += (const cMatrix &other);
  //! Resta casilla a casilla todos los componentes de la matriz y lo guarda en la matriz actual
  /*!
	\param other Matriz con la que se opera.
	\return Devuelve una referencia a la matriz actual.
  */
  inline cMatrix    &operator -= (const cMatrix &other);
  //! Multiplica todos los componentes de la matriz por un numero
  /*!
	\param value Valor por el que se multiplicara cada componente
	\return Devuelve una referencia a la matriz actual.
  */
  inline cMatrix    &operator *= (float  value);
  //! Divide todos los componentes de la matriz por un numero
  /*!
	\param value Valor por el que se dividira cada componente
	\return Devuelve una referencia a la matriz actual.
  */
  inline cMatrix    &operator /= (float  value);
  //! Multiplica las dos matrices
  /*!
	\param other Matriz por la que se va a multiplicar
	\return Devuelve una referencia a la matriz actual.
  */
  cMatrix           &operator *= (const cMatrix &other);

  // Functions
  //! Traspone la matriz actual. Si la matriz solo tiene rotacion, la traspuesta y la inversa es la misma matriz, por lo que se puede usar esta funcion para calcular una inversa muy rapida.
  /*!
	\return Devuelve una referencia a la matriz actual.
	\sa Invert
  */
  inline cMatrix &Transpose(void);
  //! Invierte la matriz actual. Esta operacion es costosa, si la matriz solo tiene rotacion esta operacion se puede reemplazar por la de transposicion.
  /*!
	\return Devuelve una referencia a la matriz actual.
	\sa Transpose
  */
  cMatrix        &Invert(void);

   //! Devuelve un puntero a la memoria de la matriz
  /*!
	\return Puntero a la memoria
  */
  inline float        *AsFloatPointer( ) { return &rows[0].x; }
  //! Devuelve un puntero a la memoria de la matriz
  /*!
	\return Puntero a la memoria
  */
  inline const float  *AsFloatPointer( ) const { return &rows[0].x; }

  //! Carga la identidad en la matriz actual
  /*!
	\return Referencia a la matriz actual
  */
  inline cMatrix &LoadIdentity(void);
  //! Carga una matriz de traslacion en la matriz actual
  /*!
	\param translation Traslacion que se quiere usar para componer la matriz
	\return Referencia a la matriz actual
  */
  inline cMatrix &LoadTranslation( const cVec3 &translation );
  //! Carga una matriz de escalado en la matriz actual
  /*!
	\param scale Escala que se quiere usar para componer la matriz. La escala es uniforme en los 3 ejes
	\return Referencia a la matriz actual
  */
  inline cMatrix &LoadScale( float  scale );
  //! Carga una matriz de escalado en la matriz actual
  /*!
	\param scale Escala que se quiere usar para componer la matriz. El valor X corresponde a la escala en el eje X, el valor Y corresponde a la escala en el eje Y y el valor Z corresponde a la escala en el eje Z
	\return Referencia a la matriz actual
  */
  inline cMatrix &LoadScale( const cVec3 &scale );
  //! Carga una matriz de rotacion en la matriz actual
  /*!
	\param axis Eje sobre el que se quiere realizar la rotacion
	\param rad Grados que se quiere que la matriz rote
	\return Referencia a la matriz actual
  */
  cMatrix &LoadRotation( const cVec3 &axis, float  rad );

  // ----------------
  //! Genera una matriz de proyeccion en perspectiva basada en un campo de vista
/*!
	\param fovy_rad Campo de vista en la direccion de y, en radianes.
	\param aspect Relacion de aspecto, que se define como el ancho del espacio de la vista dividido por el alto.
	\param znear Valor z del plano de vista proximo.
	\param zfar Valor z del plano de vista lejano.
	\return Devuelve una referencia a la propia matriz
*/
  inline cMatrix &LoadPerpective( float  fovy_rad, float  aspect, float  znear, float  zfar );
//! Genera una matriz de proyeccion ortogonal
/*!
	\param left Valor minimo de x del volumen de vista.
	\param right Valor maximo de x del volumen de vista.
	\param bottom Valor minimo de y del volumen de vista.
	\param top Valor maximo de y del volumen de vista.
	\param znear Valor minimo de z del volumen de vista.
	\param zfar Valor maximo de z del volumen de vista.
	\return Devuelve una referencia a la propia matriz
*/
  inline cMatrix &LoadOrtho( float  left, float  right, float  bottom, float  top, float  znear, float  zfar );
//! Genera una matriz de vista (enfoque)
/*!
	\param position Posicion de la camara.
	\param target Posicion a la que apunta la camara.
	\param upWorld Direccion vertical del mundo actual. Normalmente es (0,1,0)
	\return Devuelve una referencia a la propia matriz
*/
  inline cMatrix &LoadLookAt( const cVec3 &position, const cVec3 &target, const cVec3 &upWorld = cVec3(0,1,0) );
// -------------------------------------------------------
//! Devuelve la posicion de la matriz
/*!
	\return Posicion de la matriz
*/
  inline cVec3 GetPosition( ) const;
// -------------------------------------------------------
//! Devuelve el vector front resultante de la transformacion de la matriz
//! Si tuvieramos unos ejes XYZ y le aplicaramos la transformacion de esta matriz, el vector front seria igual a la direccion a la que apuntaria Z una vez transformada.
/*!
	\return Vector front de la matriz
*/
  inline cVec3 GetFront(  ) const;
// -------------------------------------------------------
//! Devuelve el vector up resultante de la transformacion de la matriz
//! Si tuvieramos unos ejes XYZ y le aplicaramos la transformacion de esta matriz, el vector up seria igual a la direccion a la que apuntaria Y una vez transformada.
/*!
	\return Vector up de la matriz
*/
  inline cVec3 GetUp( ) const;
// -------------------------------------------------------
//! Devuelve el vector left resultante de la transformacion de la matriz
//! Si tuvieramos unos ejes XYZ y le aplicaramos la transformacion de esta matriz, el vector left seria igual a la direccion a la que apuntaria X una vez transformada (o -X dependiendo si las matrices son left hand or right hand).
/*!
	\return Vector left de la matriz
*/
  inline cVec3 GetLeft( ) const;
// -------------------------------------------------------
//! Devuelve el vector right resultante de la transformacion de la matriz
//! Si tuvieramos unos ejes XYZ y le aplicaramos la transformacion de esta matriz, el vector right seria igual a la direccion a la que apuntaria -X una vez transformada (o X dependiendo si las matrices son left hand or right hand).
/*!
	\return Vector right de la matriz
*/
  inline cVec3 GetRight( ) const;
// -------------------------------------------------------
//! Permite cambiar la posicion de la matriz
/*!
	\param pos Posicion que se quiere establecer en la matriz
*/
  inline void SetPosition( const cVec3 &pos );
// -------------------------------------------------------
//! Permite cambiar los vectores de rotacion de la matriz. Los vectores deben ser perpendiculares entre si. Para representar una rotacion, los vectores deben estar normalizados.
/*!
	\param left Nuevo vector left de la matriz
	\param up Nuevo vector up de la matriz
	\param front Nuevo vector front de la matriz
*/
  inline void SetAxisLeftUpFront( const cVec3 &left, const cVec3 &up, const cVec3 &front );
// -------------------------------------------------------
//! Permite cambiar los vectores de rotacion de la matriz. Los vectores deben ser perpendiculares entre si. Para representar una rotacion, los vectores deben estar normalizados.
/*!
	\param right Nuevo vector left de la matriz
	\param up Nuevo vector up de la matriz
	\param front Nuevo vector front de la matriz
*/
  inline void setAxisRightUpFront( const cVec3 &right, const cVec3 &up, const cVec3 &front );
};

// Externs
//! Comprueba si dos matrices son iguales, comparando componente a componente
/*!
	\param m1 Operando 1
	\param m2 Operando 1
	\return Devuelve si la condicion es cierta
*/
inline bool     operator == (const cMatrix &m1, const cMatrix &m2);
//! Comprueba si al menos una componente de las dos matrices es distinto, comparando componente a componente
/*!
	\param m1 Operando 1
	\param m2 Operando 1
	\return Devuelve si la condicion es cierta
*/
inline bool     operator != (const cMatrix &m1, const cMatrix &m2);
//! Suma dos matrices, componente a componente
/*!
	\param m1 Operando 1
	\param m2 Operando 1
	\return Devuelve la matriz resultante
*/
inline cMatrix operator +  (const cMatrix &m1, const cMatrix &m2);
//! Resta dos matrices, componente a componente
/*!
	\param m1 Operando 1
	\param m2 Operando 1
	\return Devuelve la matriz resultante
*/
inline cMatrix operator -  (const cMatrix &m1, const cMatrix &m2);
//! Multiplica dos matrices
/*!
	\param m1 Operando 1
	\param m2 Operando 1
	\return Devuelve la matriz resultante
*/
inline cMatrix operator *  (const cMatrix &m1, const cMatrix &m2);
//! Multiplica todas las componentes de una matriz por un valor flotante
/*!
	\param other Matriz
	\param value Valor Flotante
	\return Devuelve la matriz resultante
*/
inline cMatrix operator *  (const cMatrix &other, float  value);
//! Multiplica todas las componentes de una matriz por un valor flotante
/*!
	\param value Valor Flotante
	\param other Matriz
	\return Devuelve la matriz resultante
*/
inline cMatrix operator *  (float  value, const cMatrix &other);
//! Divide todas las componentes de una matriz por un valor flotante
/*!
	\param other Matriz
	\param value Valor Flotante
	\return Devuelve la matriz resultante
*/
inline cMatrix operator /  (const cMatrix &other, float  value);

#ifdef ROW_AXIS
//! Transforma un punto aplicado una matriz de transformacion
/*!
   \param out Punto resultante ya transformado
	\param point Punto que se desea transformar
	\param matrix Matriz de transformacion
*/
inline void TransformPoint( cVec3 &out, const cVec3 &point, const cMatrix &matrix );
//! Transforma una normal aplicado una matriz de transformacion
/*!
   \param out Normal resultante ya transformado
	\param normal Normal que se desea transformar
	\param matrix Matriz de transformacion
*/
inline void TransformNormal( cVec3 &out, const cVec3 &normal, const cMatrix &matrix );
//! Tranforma unas coordenadas representadas por vector4 aplicando una matriz de transformacion
/*!
	\param out Vec4 resultante ya transformado
	\param coords Vec4 que se desea transformar
	\param matrix Matriz de transformacion
*/
inline void TransformCoords( cVec4 &out, const cVec4 &coords, const cMatrix &matrix );
#else
//! Transforma un punto aplicado una matriz de transformacion
/*!
   \param out Punto resultante ya transformado
	\param point Punto que se desea transformar
	\param matrix Matriz de transformacion
*/
inline void TransformPoint( cVec3 &out, const cMatrix &matrix, const cVec3 &point );
//! Transforma una normal aplicado una matriz de transformacion
/*!
   \param out Normal resultante ya transformado
	\param normal Normal que se desea transformar
	\param matrix Matriz de transformacion
*/
inline void TransformNormal( cVec3 &out, const cMatrix &matrix, const cVec3 &normal );
//! Tranforma unas coordenadas representadas por vector4 aplicando una matriz de transformacion
/*!
	\param out Vec4 resultante ya transformado
	\param coords Vec4 que se desea transformar
	\param matrix Matriz de transformacion
*/
inline void TransformCoords( cVec4 &out, const cMatrix &matrix, const cVec4 &coords );
#endif

// -------------------------------------------------------
// Operators
inline cVec4 &cMatrix::operator [] (unsigned      index) {
  assert (index < 4);
  return rows[index];
}

inline const cVec4 &cMatrix::operator [] (unsigned      index) const {
  assert (index < 4);
  return rows[index];
}

inline float & cMatrix::operator() (unsigned      i, unsigned      j) {
  assert(i > 0 && i <= 4); // Bounds checking for rows
  assert(j > 0 && j <= 4); // Bounds checking for columns
  return rows[i-1][j-1];   // Access appropriate value
}

inline const float & cMatrix::operator() (unsigned      i, unsigned      j) const {
  assert(i > 0 && i <= 4); // Bounds checking for rows
  assert(j > 0 && j <= 4); // Bounds checking for columns
  return rows[i-1][j-1];   // Access appropriate value
}

inline cMatrix &cMatrix::operator += (const cMatrix &other) {
  rows[0] += other[0];
  rows[1] += other[1];
  rows[2] += other[2];
  rows[3] += other[3];
  return *this;
}

inline cMatrix &cMatrix::operator -= (const cMatrix &other) {
  rows[0] -= other[0];
  rows[1] -= other[1];
  rows[2] -= other[2];
  rows[3] -= other[3];
  return *this;
}

inline cMatrix &cMatrix::operator *= (float  value) {
  rows[0] *= value;
  rows[1] *= value;
  rows[2] *= value;
  rows[3] *= value;
  return *this;
}

inline cMatrix &cMatrix::operator /= (float  value) {
  assert( value != 0.0f );
  float  inv = 1.0f / value;
  rows[0] *= inv;
  rows[1] *= inv;
  rows[2] *= inv;
  rows[3] *= inv;
  return *this;
}

// -------------------------------------------------------
// Functions
inline cMatrix &cMatrix::Transpose(void) {
  cMatrix temp = (*this);
  rows[0].Set( temp[0][0], temp[1][0], temp[2][0], temp[3][0] );
  rows[1].Set( temp[0][1], temp[1][1], temp[2][1], temp[3][1] );
  rows[2].Set( temp[0][2], temp[1][2], temp[2][2], temp[3][2] );
  rows[3].Set( temp[0][3], temp[1][3], temp[2][3], temp[3][3] );

  //for (unsigned      c = 0; c < 4; ++c) {
  //    for (unsigned      r = c + 1; r < 4; ++r) {
  //        float  tmp  = rows[r][c];
  //        rows[r][c] = rows[c][r];
  //        rows[c][r] = tmp;
  //    } 
  //}
  return *this;
}

inline cMatrix &cMatrix::LoadIdentity(void) {
  rows[0].Set(1.0f, 0.0f, 0.0f, 0.0f);
  rows[1].Set(0.0f, 1.0f, 0.0f, 0.0f);
  rows[2].Set(0.0f, 0.0f, 1.0f, 0.0f);
  rows[3].Set(0.0f, 0.0f, 0.0f, 1.0f);
  return *this;
}

inline cMatrix &cMatrix::LoadTranslation( const cVec3 &translation ) {
  LoadIdentity( );
  SetPosition( translation );
  return *this;
}

inline cMatrix &cMatrix::LoadScale( float  scale ) {
  assert( scale != 0.0f );
  rows[0].Set(scale, 0.0f,  0.0f,  0.0f);
  rows[1].Set(0.0f,  scale, 0.0f,  0.0f);
  rows[2].Set(0.0f,  0.0f,  scale, 0.0f);
  rows[3].Set(0.0f,  0.0f,  0.0f,  1.0f);
  return *this;
}

inline cMatrix &cMatrix::LoadScale( const cVec3 &scale ) {
  assert( !scale.IsZero( ) );
  rows[0].Set(scale.x, 0.0f,    0.0f,    0.0f);
  rows[1].Set(0.0f,    scale.y, 0.0f,    0.0f);
  rows[2].Set(0.0f,    0.0f,    scale.z, 0.0f);
  rows[3].Set(0.0f,    0.0f,    0.0f,    1.0f);
  return *this;
}

// -------------------------------------------------------
// Externs
inline bool operator == (const cMatrix &m1, const cMatrix &m2) {
  return((m1[0] == m2[0]) && (m1[1] == m2[1]) &&
      (m1[2] == m2[2]) && (m1[3] == m2[3]));
}

inline bool operator != (const cMatrix &m1, const cMatrix &m2) {
  return((m1[0] != m2[0]) || (m1[1] != m2[1]) ||
      (m1[2] != m2[2]) || (m1[3] != m2[3]));
}

inline cMatrix operator + (const cMatrix &m1, const cMatrix &m2) {
  cMatrix ret(m1);
  ret += m2;
  return ret;
}

inline cMatrix operator - (const cMatrix &m1, const cMatrix &m2) {
  cMatrix ret(m1);
  ret -= m2;
  return ret;
}

inline cMatrix operator * (const cMatrix &m1, const cMatrix &m2) {
  cMatrix ret(m1);
  ret *= m2;
  return ret;
}

inline cMatrix operator * (const cMatrix &other, float  f) {
  cMatrix ret(other);
  ret *= f;
  return ret;
}

inline cMatrix operator * (float  f, const cMatrix &other) {
  cMatrix ret(other);
  ret *= f;
  return ret;
}

inline cMatrix operator / (const cMatrix &other, float  f) {
  cMatrix ret(other);
  ret /= f;
  return ret;
}

#ifdef ROW_AXIS
#include "Matrix44Row.h"
#else
#include "Matrix44Col.h"
#endif // ROW_AXIS

#endif // MATRIX_44_H_
