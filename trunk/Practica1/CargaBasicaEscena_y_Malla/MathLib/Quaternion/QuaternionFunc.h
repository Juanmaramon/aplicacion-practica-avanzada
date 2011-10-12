#ifndef QUATERNION_FUNC_H_
#define QUATERNION_FUNC_H_

#include "Quaternion.h"

//! Transforma un punto por un quaternion
/*!
   Transforma un punto por un quaternion
   \param out Punto resultante de la transformacion
   \param q Quaternion
   \param v Punto a transformar
*/
inline void TransformPoint( cVec3 &out, const cQuaternion &q, const cVec3 &v );

//! Calcula el producto escalar de dos quaterniones
/*!
   Calcula el producto escalar de dos quaterniones
   \param first Primer operando
   \param second Segundo operando
   \return Resultado del producto escalar
*/
inline float  Dot(const cQuaternion &first, const cQuaternion &second);

//! Calcula la interpolacion lineal de dos quaterniones
/*!
   Calcula la interpolacion lineal de dos quaterniones
   \param out Quaternion resultante
   \param from Primer operando
   \param To Segundo operando
   \param t Factor de interpolacion
*/
inline void Lerp(cQuaternion &out, const cQuaternion &from, const cQuaternion &To, float  t);
//! Calcula la interpolacion esferica de dos quaterniones
/*!
   Calcula la interpolacion esferica de dos quaterniones
   \param out Quaternion resultante
   \param from Primer operando
   \param To Segundo operando
   \param t Factor de interpolacion
*/
void Slerp(cQuaternion &out, const cQuaternion &from, const cQuaternion &To, float  t);
//! Genera el quaternion resultante de rotar un vector hasta el otro
/*!
   Genera el quaternion resultante de rotar un vector hasta el otro
   \param out Quaternion resultante
   \param v0 Primer vector
   \param v1 Primer vector
*/
inline void RotationArc(cQuaternion &out, const cVec3 &v0,const cVec3 &v1);

inline void TransformPoint( cVec3 &out, const cQuaternion &q, const cVec3 &v ) {
  cQuaternion p_quat(v.x, v.y, v.z, 0.0f);
  cQuaternion q_inv = q;
  q_inv.Invert( );

  // Point is in the right
  // final_point = q1 * point
  // final_point = (q1 * q2) * point -> so, first rotation is q2 and second rotation is q1

  p_quat = q * p_quat * q_inv;
 out.x = p_quat.x;
 out.y = p_quat.y;
 out.z = p_quat.z;
}

inline float  Dot(const cQuaternion &first, const cQuaternion &second) {
  return first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;
}

inline void Lerp(cQuaternion &out, const cQuaternion &from, const cQuaternion &To, float  t) {
  out = from*(1.0f-t) + To*t;
  out.Normalize();
}

inline void RotationArc(cQuaternion &out, const cVec3 &v0,const cVec3 &v1) {
  cVec3 temp;
  Cross(temp,v0,v1);

  float    d = Dot(v0,v1);
  float    s = sqrtf((1+d)*2);
  assert( s != 0 );
  float  inv_s = 1.0f / s;

  out.Set( temp.x * inv_s, temp.y * inv_s, temp.z * inv_s, s * 0.5f );
}

#endif // QUATERNION_FUNC_H_
