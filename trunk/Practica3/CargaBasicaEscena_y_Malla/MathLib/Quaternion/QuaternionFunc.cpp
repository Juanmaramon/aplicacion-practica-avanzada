#include "QuaternionFunc.h"

void Slerp(cQuaternion &out, const cQuaternion &from, const cQuaternion &To, float  t) {
  cQuaternion temp;

  float  cosO = Dot(from, To);

  if (cosO < 0.0f) {
      cosO = -cosO;
      temp = -To;
  }
  else {
      temp = To;
  }

  if ((1.0f - cosO) > EPSILON) {
      float  omega, sinO;
      float  scale0, scale1;

      omega  = acosf(cosO);
      sinO   = sinf(omega);
      assert( sinO != 0.0f );
      scale0 = sinf((1.0f - t) * omega) / sinO;
      scale1 = sinf(t * omega) / sinO;

      out = from*scale0 + temp*scale1;
  }
  else {
      Lerp( out, from, temp, t );
  }
}

