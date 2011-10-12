#include "Matrix.h"
#include "../Quaternion/Quaternion.h"

cMatrix::cMatrix(float  value) {
  rows[0].Set(value, value, value, value);
  rows[1].Set(value, value, value, value);
  rows[2].Set(value, value, value, value);
  rows[3].Set(value, value, value, value);
}

cMatrix::cMatrix(const cVec4 &v0,const cVec4 &v1,const cVec4 &v2, const cVec4 &v3) {
  rows[0] = v0;
  rows[1] = v1;
  rows[2] = v2;
  rows[3] = v3;
}

// -------------------------------------------------------
cMatrix &cMatrix::operator *= (const cMatrix &other) {
  cMatrix t;
  for (unsigned      r = 0; r < 4; ++r) {
      for (unsigned      c = 0; c < 4; ++c) {
          float  f = 0.0f;
          f += (rows[r][0] * other[0][c]);
          f += (rows[r][1] * other[1][c]);
          f += (rows[r][2] * other[2][c]);
          f += (rows[r][3] * other[3][c]);
          t[r][c] = f;
      }
  }
  (*this) = t;
  return *this;
}

// -------------------------------------------------------
cMatrix &cMatrix::Invert(void) {
  static const int N = 4;
  //int *index;  index = new int [N+1];
  int index[N+1];

  cMatrix A(*this);
  cMatrix &Ainv = *this;

  int i, j, k;
  cMatrix b;     // Scale factor and work array
  cVec4 scale;

  //* Matrix b is initialized to the identity matrix
  b.LoadIdentity();

  //* Set scale factor, scale(i) = max( |a(i,j)| ), for each row
  for( i=1; i<=N; i++ ) {
      index[i] = i;              // Initialize row index list
      float  scalemax = 0.;
      for( j=1; j<=N; j++ ) 
          scalemax = (scalemax > fabs(A(i,j))) ? scalemax : fabs(A(i,j));
      scale(i) = scalemax;
  }

  //* Loop over rows k = 1, ..., (N-1)
  int signDet = 1;
  for( k=1; k<=N-1; k++ ) {
      //* Select pivot row from max( |a(j,k)/s(j)| )
      float  ratiomax = 0.0f;
      int jPivot = k;
      for( i=k; i<=N; i++ ) {
          float  ratio = fabs(A(index[i],k))/scale(index[i]);
          if( ratio > ratiomax ) {
              jPivot=i;
              ratiomax = ratio;
          }
      }
      //* Perform pivoting using row index list
      int indexJ = index[k];
      if( jPivot != k ) {              // Pivot
          indexJ = index[jPivot];
          index[jPivot] = index[k];   // Swap index jPivot and k
          index[k] = indexJ;
          signDet *= -1;              // Flip sign of determinant
      }
      //* Perform forward elimination
      for( i=k+1; i<=N; i++ ) {
          float  coeff = A(index[i],k)/A(indexJ,k);
          for( j=k+1; j<=N; j++ )
              A(index[i],j) -= coeff*A(indexJ,j);
          A(index[i],k) = coeff;
          for( j=1; j<=N; j++ ) 
              b(index[i],j) -= A(index[i],k)*b(indexJ,j);
      }
  }
  //* Compute determinant as product of diagonal elements
  float  determ = (float )signDet;       // Sign of determinant
  for( i=1; i<=N; i++ )
      determ *= A(index[i],i);

  //* Perform backsubstitution
  for( k=1; k<=N; k++ ) {
      Ainv(N,k) = b(index[N],k)/A(index[N],N);
      for( i=N-1; i>=1; i--) {
          float  sum = b(index[i],k);
          for( j=i+1; j<=N; j++ )
              sum -= A(index[i],j)*Ainv(j,k);
          Ainv(i,k) = sum/A(index[i],i);
      }
  }

  //delete [] index;    // Release allocated memory
  //return( determ );                
  return( Ainv );                
}

// -------------------------------------------------------
cMatrix &cMatrix::LoadRotation( const cVec3 &axis, float  rad ) {
  cQuaternion quat;
  quat.LoadRotationAxis( axis, rad );
  quat.AsMatrix( (*this) );
  return *this;
}

