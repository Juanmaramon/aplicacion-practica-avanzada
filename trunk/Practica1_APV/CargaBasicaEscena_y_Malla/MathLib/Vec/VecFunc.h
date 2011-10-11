#ifndef VECTOR_FUNC_H_
#define VECTOR_FUNC_H_

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

// Dot Products
inline float  Dot(const cVec2 &v1, const cVec2 &v2) { return v1.x*v2.x + v1.y*v2.y; }
inline float  Dot(const cVec3 &v1, const cVec3 &v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }
inline float  Dot(const cVec4 &v1, const cVec4 &v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w; }

// Cross Product
inline void Cross(cVec3 &out, const cVec3 &v1, const cVec3 &v2) {
  out.Set( v1.y*v2.z - v1.z*v2.y,
      v1.z*v2.x - v1.x*v2.z,
      v1.x*v2.y - v1.y*v2.x );
}

// Swap vectors
inline void Swap(cVec2 &v1, cVec2 &v2) { cVec2 temp(v1); v1 = v2; v2 = temp; }
inline void Swap(cVec3 &v1, cVec3 &v2) { cVec3 temp(v1); v1 = v2; v2 = temp; }
inline void Swap(cVec4 &v1, cVec4 &v2) { cVec4 temp(v1); v1 = v2; v2 = temp; }

#endif // VECTOR_FUNC_H_
