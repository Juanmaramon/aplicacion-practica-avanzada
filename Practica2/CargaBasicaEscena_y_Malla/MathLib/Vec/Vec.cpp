#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

// ----------------------------------------------------------
cVec2::cVec2(const cVec3 &vec3): x(vec3.x), y(vec3.y) {}
cVec2::cVec2(const cVec4 &vec4): x(vec4.x), y(vec4.y) {}

void cVec2::StoreMax( const cVec2 &p ) {
  if( p.x > x ) x = p.x;
  if( p.y > y ) y = p.y;
}
void cVec2::StoreMin( const cVec2 &p ) {
  if( p.x < x ) x = p.x;
  if( p.y < y ) y = p.y;
}

// ----------------------------------------------------------
cVec3::cVec3(const cVec2 &vec2, float  lz): x(vec2.x), y(vec2.y), z(lz) {}
cVec3::cVec3(const cVec4 &vec4): x(vec4.x), y(vec4.y), z(vec4.z) {}

void cVec3::StoreMax( const cVec3 &p ) {
  if( p.x > x ) x = p.x;
  if( p.y > y ) y = p.y;
  if( p.z > z ) z = p.z;
}
void cVec3::StoreMin( const cVec3 &p ) {
  if( p.x < x ) x = p.x;
  if( p.y < y ) y = p.y;
  if( p.z < z ) z = p.z;
}

// ----------------------------------------------------------
cVec4::cVec4(const cVec2 &vec2, float  lz, float  lw): x(vec2.x), y(vec2.y), z(lz), w(lw) {}
cVec4::cVec4(const cVec3 &vec3, float  lw): x(vec3.x), y(vec3.y), z(vec3.z), w(lw) {}

void cVec4::StoreMax( const cVec4 &p ) {
  if( p.x > x ) x = p.x;
  if( p.y > y ) y = p.y;
  if( p.z > z ) z = p.z;
  if( p.w > w ) w = p.w;
}
void cVec4::StoreMin( const cVec4 &p ) {
  if( p.x < x ) x = p.x;
  if( p.y < y ) y = p.y;
  if( p.z < z ) z = p.z;
  if( p.w < w ) w = p.w;
}
