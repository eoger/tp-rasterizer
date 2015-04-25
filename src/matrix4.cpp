// Math 
// Copyright LIRIS/Geomod - Eric Galin

#include "matrix.h"

/*!
\class Matrix4 matrix.h
\brief This class implements a 4x4 double matrix. 

Components are stored in a single dimension array, starting from 
element a<SUB>00</SUB> and sorting components
by column.

The diagonal elements of a matrix A are A<SUB>00</SUB>=A[0], A<SUB>11</SUB>=A[5], A<SUB>22</SUB>=A[10], A<SUB>33</SUB>=A[15].
The terms corresponding to the translation vector are A[12], A[13], A[14].
\ingroup Math
\changed 05.01.24
*/

const Matrix4 Matrix4::Zero(0.0);

const Matrix4 Matrix4::Identity(1.0);

/*!
\brief Creates a null matrix.
*/
void Matrix4::Null()
{
  for (int i=0;i<16;i++)
  {
    r[i]=0.0;
  }
}

/*!
\brief Creates an identity matrix.
*/
void Matrix4::Id()
{
  Null();
  r[0]=r[5]=r[10]=r[15]=1.0;
}

/*!
\brief Create an scaling matrix with the same diagonal terms (the
last term of the matrix is set to 1.0).
\param a Value of diagonal entries.
*/
Matrix4::Matrix4(const double& a)
{
  Null();
  r[0]=r[5]=r[10]=a;
  r[15]=1.0;
}

/*!
\brief Create a homogeneous diagonal matrix with diagonal terms 
set to the vector entries. Last diagonal entry is set to 1.0.
\param a Vector of diagonal entries.
*/
Matrix4::Matrix4(const Vector& a)
{
  Null();
  r[0]=a[0];
  r[5]=a[1];
  r[10]=a[2];
  r[15]=1.0;
}

/*!
\brief Create an homogeneous matrix from a simple 3x3 matrix (the translation terms
and the shear terms are set to 0.0).
\param a Matrix.
*/
Matrix4::Matrix4(const Matrix& a)
{
  // Rotation and scale
  r[0]=a[0];
  r[1]=a[1];
  r[2]=a[2];
  r[4]=a[3];
  r[5]=a[4];
  r[6]=a[5];
  r[8]=a[6];
  r[9]=a[7];
  r[10]=a[8];
  // Translation
  r[3]=r[7]=r[11]=0.0;
  // Shear
  r[12]=r[13]=r[14]=0.0;
  // Scale
  r[15]=1.0;
}

/*!
\brief Create an homogeneous matrix from a simple 3x3 matrix and a translation vector 
(the shear terms are set to 0.0).
\param a Matrix.
\param t Translation vector.
*/
Matrix4::Matrix4(const Matrix& a,const Vector& t)
{
  // Rotation and scale
  r[0]=a[0];
  r[1]=a[1];
  r[2]=a[2];
  r[4]=a[3];
  r[5]=a[4];
  r[6]=a[5];
  r[8]=a[6];
  r[9]=a[7];
  r[10]=a[8];
  // Translation
  r[12]=t[0];
  r[13]=t[1];
  r[14]=t[2];
  // Shear
  r[3]=r[7]=r[11]=0.0;
  // Scale
  r[15]=1.0;
}

/*!
\brief Create an homogeneous matrix from a simple 3x3 matrix, a translation vector and
a shear vector.
\param a Matrix.
\param t Translation vector.
\param s Shear vector.
*/
Matrix4::Matrix4(const Matrix& a,const Vector& t,const Vector& s)
{
  // Rotation and scale
  r[0]=a[0];
  r[1]=a[1];
  r[2]=a[2];
  r[4]=a[3];
  r[5]=a[4];
  r[6]=a[5];
  r[8]=a[6];
  r[9]=a[7];
  r[10]=a[8];
  // Translation
  r[12]=t[0];
  r[13]=t[1];
  r[14]=t[2];
  // Shear
  r[3]=s[0];
  r[6]=s[1];
  r[9]=s[2];
  // Scale
  r[15]=1.0;
}

/*!
\brief Transpose a matrix.
*/
Matrix4 T(const Matrix4& a)
{
  Matrix4 t;
  t[0]=a[0];
  t[5]=a[5];
  t[10]=a[10];
  t[15]=a[15];

  t[1]=a[4];
  t[2]=a[8];
  t[3]=a[12];
  t[6]=a[9];
  t[7]=a[13];
  t[11]=a[14];

  t[4]=a[1];
  t[8]=a[2];
  t[12]=a[3];
  t[9]=a[6];
  t[13]=a[7];
  t[14]=a[11];

  return t;
}

/*!
\brief Returns the opposite of a matrix -A.
*/
Matrix4 Matrix4::operator-() const
{
  Matrix4 n;
  for (int i=0;i<16;i++)
  {
    n.r[i]=-r[i];
  }
  return n;
}

/*!
\brief Multiplication.
*/
Matrix4 operator*(const Matrix4& u,const Matrix4& v)
{
  Matrix4 a;
  for (int i=0;i<4;i++)
  {
    int k=i<<2;
    for (int j=0;j<4;j++)
    {
      a.r[k+j]=u.r[j]*v.r[k]+u.r[4+j]*v.r[k+1]+u.r[8+j]*v.r[k+2]+u.r[12+j]*v.r[k+3];
    }
  }
  return a;
}

/*!
\brief Destructive multiplication.
*/
Matrix4& Matrix4::operator*=(const Matrix4& v)
{
  Matrix4 r;

  r=(*this)*v;
  *this=r;
  return *this;
}


Vector operator*(const Matrix4& u, const Vector& v)
{
  double w=1.0/(v[0]*u.r[3]+v[1]*u.r[7]+v[2]*u.r[11]+u.r[15]);

  Vector r;
  for (int i=0;i<3;i++)
  {
    r[i]=(v[0]*u.r[i]+v[1]*u.r[4+i]+v[2]*u.r[8+i]+u.r[12+i])*w;
  }
  return r;
}

/*!
\brief Computes the inverse of a matrix A<SUP>-1</SUP>. 

This function returns the null matrix if A cannot be inverted. 
The threshold value involved in the singular matrix detection is 
set to 10<SUP>-18</SUP>.

\param m Argument matrix.
*/
Matrix4 Inverse(const Matrix4& m)
{
  double d00=m[5]*m[10]*m[15]+m[9]*m[14]*m[7]+m[13]*m[6]*m[11]-m[7]*m[10]*m[13]-m[11]*m[14]*m[5]-m[15]*m[6]*m[9];
  double d01=m[1]*m[10]*m[15]+m[9]*m[14]*m[3]+m[13]*m[2]*m[11]-m[3]*m[10]*m[13]-m[11]*m[14]*m[1]-m[15]*m[2]*m[9];
  double d02=m[1]*m[6]*m[15]+m[5]*m[14]*m[3]+m[13]*m[2]*m[7]-m[3]*m[6]*m[13]-m[7]*m[14]*m[1]-m[15]*m[2]*m[5];
  double d03=m[1]*m[6]*m[11]+m[5]*m[10]*m[3]+m[9]*m[2]*m[7]-m[3]*m[6]*m[9]-m[7]*m[10]*m[1]-m[11]*m[2]*m[5];

  // Test if singular matrix
  double d=m[0]*d00-m[4]*d01+m[8]*d02-m[12]*d03;

  if (fabs(d)<1.0e-18)
  {
    return Matrix4::Zero;
  }

  // Inverse
  d=1.0/d;

  double d10=m[4]*m[10]*m[15]+m[8]*m[14]*m[7]+m[12]*m[6]*m[11]-m[7]*m[10]*m[12]-m[11]*m[14]*m[4]-m[15]*m[6]*m[8];
  double d11=m[0]*m[10]*m[15]+m[8]*m[14]*m[3]+m[12]*m[2]*m[11]-m[3]*m[10]*m[12]-m[11]*m[14]*m[0]-m[15]*m[2]*m[8];
  double d12=m[0]*m[6]*m[15]+m[4]*m[14]*m[3]+m[12]*m[2]*m[7]-m[3]*m[6]*m[12]-m[7]*m[14]*m[0]-m[15]*m[2]*m[4];
  double d13=m[0]*m[6]*m[11]+m[4]*m[10]*m[3]+m[8]*m[2]*m[7]-m[3]*m[6]*m[8]-m[7]*m[10]*m[0]-m[11]*m[2]*m[4];

  double d20=m[4]*m[9]*m[15]+m[8]*m[13]*m[7]+m[12]*m[5]*m[11]-m[7]*m[9]*m[12]-m[11]*m[13]*m[4]-m[15]*m[5]*m[8];
  double d21=m[0]*m[9]*m[15]+m[8]*m[13]*m[3]+m[12]*m[1]*m[11]-m[3]*m[9]*m[12]-m[11]*m[13]*m[0]-m[15]*m[1]*m[8];
  double d22=m[0]*m[5]*m[15]+m[4]*m[13]*m[3]+m[12]*m[1]*m[7]-m[3]*m[5]*m[12]-m[7]*m[13]*m[0]-m[15]*m[1]*m[4];
  double d23=m[0]*m[5]*m[11]+m[4]*m[9]*m[3]+m[8]*m[1]*m[7]-m[3]*m[5]*m[8]-m[7]*m[9]*m[0]-m[11]*m[1]*m[4];

  double d30=m[4]*m[9]*m[14]+m[8]*m[13]*m[6]+m[12]*m[5]*m[10]-m[6]*m[9]*m[12]-m[10]*m[13]*m[4]-m[14]*m[5]*m[8];
  double d31=m[0]*m[9]*m[14]+m[8]*m[13]*m[2]+m[12]*m[1]*m[10]-m[2]*m[9]*m[12]-m[10]*m[13]*m[0]-m[14]*m[1]*m[8];
  double d32=m[0]*m[5]*m[14]+m[4]*m[13]*m[2]+m[12]*m[1]*m[6]-m[2]*m[5]*m[12]-m[6]*m[13]*m[0]-m[14]*m[1]*m[4];
  double d33=m[0]*m[5]*m[10]+m[4]*m[9]*m[2]+m[8]*m[1]*m[6]-m[2]*m[5]*m[8]-m[6]*m[9]*m[0]-m[10]*m[1]*m[4];

  // Create inverse
  Matrix4 r;   

  r[0]=d00*d;  r[4]=-d10*d; r[8]=d20*d; r[12]=-d30*d;
  r[1]=-d01*d; r[5]= d11*d; r[9] =-d21*d; r[13]= d31*d;
  r[2]= d02*d; r[6]=-d12*d; r[10]= d22*d; r[14]=-d32*d;
  r[3]=-d03*d; r[7]= d13*d; r[11]=-d23*d; r[15]= d33*d;
  return r;
}

/*!
\brief Destructive addition.
*/
Matrix4& Matrix4::operator+= (const Matrix4& u)
{
  for (int i=0;i<16;i++)
  {
    r[i]+=u.r[i];
  }
  return *this;
}

/*!
\brief Destructive subtraction.
*/
Matrix4& Matrix4::operator-= (const Matrix4& u)
{
  for (int i=0;i<16;i++)
  {
    r[i]-=u.r[i];
  }
  return *this;
}

/*!
\brief Destructive scalar multiply.
*/
Matrix4& Matrix4::operator*= (double a)
{
  for (int i=0;i<16;i++)
  {
    r[i]*=a;
  }
  return *this;
}

/*!
\brief Destructive scalar divide.
*/
Matrix4& Matrix4::operator/= (double a)
{
  for (int i=0;i<16;i++)
  {
    r[i]*=(1/a);
  }
  return *this;
}

/*!
\brief Overloaded.
\todo Rewrite with a new constructor with 16 elements, inline member and constructor as for Matrix.
*/
Matrix4 operator+ (const Matrix4& u, const Matrix4& v)
{
  Matrix4 e=u;
  e+=v;
  return e;
}

/*!
\brief Overloaded.
*/
Matrix4 operator- (const Matrix4& u, const Matrix4& v)
{
  Matrix4 e=u;
  e-=v;
  return e;
}

/*!
\brief Overloaded.
\param s Stream.
\param matrix The matrix.
*/
ostream& operator<<(ostream& s, const Matrix4& matrix)
{
  s<<"Matrix4(";
  for (int i=0;i<4;i++)
  {
    for (int j=0;j<4;j++)
    {
      s<<matrix(i,j);
      if (i*4+j!=15)
      {
        s<<',';
      }
    }
  }
  s<<')';
  return s;
}

/*!
\brief Create a rotation matrix about an arbitrary axis.
\param v Vector axis.
\param a Angle.
*/
Matrix4 Matrix4::Rotate(const Vector& v, const double& a)
{
  Matrix4 mat;
  mat.Id();
  Vector vnorm=Normalized(v);
  double x=vnorm[0];
  double y=vnorm[1];
  double z=vnorm[2];
  double s=sin(a);
  double c=cos(a);
  double t=1.-c;
  mat(0,0)=t*x*x+c;
  mat(0,1)=t*x*y+s*z;
  mat(0,2)=t*x*z-s*y;
  mat(1,0)=t*x*y-s*z;
  mat(1,1)=t*y*y+c;
  mat(1,2)=t*y*z+s*x;
  mat(2,0)=t*x*z+s*y;
  mat(2,1)=t*y*z-s*x;
  mat(2,2)=t*z*z+c;
  return mat;
}

/*!
\brief Create a rotation matrix about the orthogonal axes.
*/
Matrix4 Matrix4::Rotate(const Vector& u) 
{ 
  return Matrix4(Matrix::Rotation(u)); 
}

/*!
\brief Creates a scaling matrix.
*/
Matrix4 Matrix4::Scale(const Vector& u)
{ 
  return Matrix4(u); 
}

/*!
\brief Creates a translation matrix.
\param t Translation vector.
*/
Matrix4 Matrix4::Translate(const Vector& t) 
{ 
  return Matrix4(Matrix(1.0),t); 
}

/*!
\brief Creates a shear matrix.
\param s Shear vector.
*/
Matrix4 Matrix4::Shear(const Vector& s) 
{ 
  return Matrix4(Matrix(1.0),Vector(0.0),s); 
}


/*!
\brief Compute the determinant of the matrix.
\param M Argument matrix.
*/
double Det(const Matrix4& M)
{
  return M(0,0)*Matrix(M(1,1),M(1,2),M(1,3),M(2,1),M(2,2),M(2,3),M(3,1),M(3,2),M(3,3)).Determinant()
    -M(1,0)*Matrix(M(0,1),M(0,2),M(0,3),M(2,1),M(2,2),M(2,3),M(3,1),M(3,2),M(3,3)).Determinant()
    +M(2,0)*Matrix(M(0,1),M(0,2),M(0,3),M(1,1),M(1,2),M(1,3),M(3,1),M(3,2),M(3,3)).Determinant()
    -M(3,0)*Matrix(M(0,1),M(0,2),M(0,3),M(1,1),M(1,2),M(1,3),M(2,1),M(2,2),M(2,3)).Determinant();
}
