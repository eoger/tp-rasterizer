// Matrix 
// Copyright LIRIS/Geomod - Eric Galin

#include "matrix.h"

/*!
\class Matrix matrix.h
\brief This class implements a 3<SUP>2</SUP> matrix. 

Operators have been 
overloaded so as to behave as expected with the Vector class. 

The constructors of this class are used to create matrices in the general case.
See the static member functions Matrix::Rotation() to create different kinds 
of rotation matrices.

Components are stored in a single dimension array, sorting components 
by column.

\ingroup Math
\changed 05.01.24
*/

double Matrix::epsilon=1.0e-8;

/*!
\brief This static member defines the null matrix.
*/
const Matrix Matrix::Zero(0.0);

/*!
\brief This static member defines the identity matrix.
*/
const Matrix Matrix::Identity(1.0);

/*!
\brief Computes the inverse of a matrix A<SUP>-1</SUP>. Recall that A<SUP>-1</SUP>
can be defined as the transposed adjoint matrix divided by the determinant. 

This function returns the null matrix if A cannot be inversed. 

The threshold value involved in the singular matrix detection
is set to 10<SUP>-18</SUP>.

\param A %Matrix. 
*/
Matrix Inverse(const Matrix& A) 
{
  //static const double epsilon=1.0e-18;

  double e=A.Determinant();

  if (fabs(e)<1.0e-18)
  {
    return Matrix::Zero;
  }   

  return T(A.Adjoint())/e;
}

/*!
\brief Multiplication.

\param u, v Input matrices.
*/
Matrix operator* (const Matrix& u, const Matrix& v)
{
  return Matrix(u[0]*v[0]+u[3]*v[1]+u[6]*v[2],u[1]*v[0]+u[4]*v[1]+u[7]*v[2],u[2]*v[0]+u[5]*v[1]+u[8]*v[2],
    u[0]*v[3]+u[3]*v[4]+u[6]*v[5],u[1]*v[3]+u[4]*v[4]+u[7]*v[5],u[2]*v[3]+u[5]*v[4]+u[8]*v[5],
    u[0]*v[6]+u[3]*v[7]+u[6]*v[8],u[1]*v[6]+u[4]*v[7]+u[7]*v[8],u[2]*v[6]+u[5]*v[7]+u[8]*v[8]);
}

/*!
\brief Destructive multiplication.
*/
Matrix& Matrix::operator*=(const Matrix& M)
{
  Matrix r=(*this)*M;
  *this=r;
  return *this;
}


/*! 
\brief Computes the inverse matrix and scales it by a double. 

This high level function calls the Matrix::Inverse() member. 
*/
Matrix operator/(const double& a,const Matrix& u)
{
  return (1.0/a)*Inverse(u);
}

/*!
\brief Create a rotation matrix given a 
vector of angles that specifies the rotation 
around each world coordinate axis.

Rotations are concatenated, starting by rotating
around the z axis, then y and eventually x.

\param v Vector of angles in radian.
*/
Matrix Matrix::Rotation(const Vector& v)
{  
  return Matrix::RotationX(v[0])*Matrix::RotationY(v[1])*Matrix::RotationZ(v[2]);
}

/*!
\brief Create a rotation matrix around the x-axis.

This static member function is provided out of efficiency as
it is much faster than any other general Matrix::Rotation() member.

\param a Angle (in radian).
*/
Matrix Matrix::RotationX(const double& a)
{  
  double c=cos(a);
  double s=sin(a);
  return Matrix(1.0,0.0,0.0,0.0,c,s,0.0,-s,c);
}

/*!
\brief Create a rotation matrix around the y-axis.

This static member function is provided out of efficiency as
it is much faster than any other general Matrix::Rotation() member.

\param a Angle (in radian).
*/
Matrix Matrix::RotationY(const double& a)
{  
  double c=cos(a);
  double s=sin(a);
  return Matrix(c,0.0,-s,0.0,1.0,0.0,s,0.0,c);
}

/*!
\brief Create a rotation matrix around the z-axis.

This static member function is provided out of efficiency as
it is much faster than any other general Matrix::Rotation() member.

\param a Angle (in radian).
*/
Matrix Matrix::RotationZ(const double& a)
{  
  double c=cos(a);
  double s=sin(a);
  return Matrix(c,s,0.0,-s,c,0.0,0.0,0.0,1.0);
}

/*!
\brief Create a rotation matrix that rotates a normalized 
vector into another one.
\param a, b Initial and final vector (should be normalized).
*/
Matrix Matrix::Rotation(const Vector& a,const Vector& b)
{  
  Matrix matrix;

  Vector v=a/b;
  double e=a*b;

  // Almost identical vectors
  if(e>1.0-epsilon)
  {
    return Matrix::Identity;
  }
  // Almost opposite vectors
  else if(e<epsilon-1.0) 
  {
    double fxx,fyy,fzz,fxy,fxz,fyz;
    double uxx,uyy,uzz,uxy,uxz,uyz;
    double lxx,lyy,lzz,lxy,lxz,lyz;

    Vector left(0.0,a[2],-a[1]);
    if (left*left<epsilon)
    {
      left[0]=-a[2]; left[1]=0.0; left[2]=a[0];
    }
    double invlen=1.0/sqrt(left*left);
    left*=invlen;
    Vector up=left/a;
    // now we have a coordinate system, i.e., a basis
    /* M=(a, up, left), and we want to rotate to:    */
    /* N=(-a, up, -left). This is done with the matrix:*/
    /* N*M^T where M^T is the transpose of M          */
    fxx=-a[0]*a[0]; fyy=-a[1]*a[1]; fzz=-a[2]*a[2];
    fxy=-a[0]*a[1]; fxz=-a[0]*a[2]; fyz=-a[1]*a[2];

    uxx=up[0]*up[0]; uyy=up[1]*up[1]; uzz=up[2]*up[2];
    uxy=up[0]*up[1]; uxz=up[0]*up[2]; uyz=up[1]*up[2];

    lxx=-left[0]*left[0]; lyy=-left[1]*left[1]; lzz=-left[2]*left[2];
    lxy=-left[0]*left[1]; lxz=-left[0]*left[2]; lyz=-left[1]*left[2];
    // Symmetric matrix
    matrix(0,0)=fxx+uxx+lxx; matrix(0,1)=fxy+uxy+lxy; matrix(0,2)=fxz+uxz+lxz;
    matrix(1,0)=matrix(0,1); matrix(1,1)=fyy+uyy+lyy; matrix(1,2)=fyz+uyz+lyz;
    matrix(2,0)=matrix(0,2); matrix(2,1)=matrix(1,2); matrix(2,2)=fzz+uzz+lzz;
  }
  else
  {
    double h=(1.0-e)/(v*v);
    double hvx=h*v[0];
    double hvz=h*v[2];
    double hvxy=hvx*v[1];
    double hvxz=hvx*v[2];
    double hvyz=hvz*v[1];
    matrix(0,0)=e+hvx*v[0]; matrix(0,1)=hvxy-v[2];    matrix(0,2)=hvxz+v[1];
    matrix(1,0)=hvxy+v[2];  matrix(1,1)=e+h*v[1]*v[1]; matrix(1,2)=hvyz-v[0];
    matrix(2,0)=hvxz-v[1];  matrix(2,1)=hvyz+v[0];    matrix(2,2)=e+hvz*v[2];
  }  
  return matrix;
}

/*!
\brief Create a rotation matrix about an arbitrary axis.

\code
Matrix R=Matrix::Rotation(Normalized(Vector(1,2,-1)),Math::DegreeToRadian(30.0));
\endcode

\param u Rotation axis, which should be unit.
\param a Rotation angle (should be in radian).
*/

Matrix Matrix::Rotation(const Vector& u,const double& a)
{  
  Vector v=sin(0.5*a)*u;
  double w=cos(0.5*a);
  double tx =2.0*v[0];
  double ty =2.0*v[1];
  double tz =2.0*v[2];
  double twx=tx*w;
  double twy=ty*w;
  double twz=tz*w;
  double txx=tx*v[0];
  double txy=ty*v[0];
  double txz=tz*v[0];
  double tyy=ty*v[1];
  double tyz=tz*v[1];
  double tzz=tz*v[2];

  return Matrix(1.0-(tyy+tzz),txy-twz,txz+twy,txy+twz,1.0-(txx+tzz),tyz-twx,txz-twy,tyz+twx,1.0-(txx+tyy));
}

/*!
\brief Destructive addition operator.
*/
Matrix& Matrix::operator+=(const Matrix& u)
{
  for (int i=0;i<9;i++)
  {
    r[i]+=u.r[i];
  }
  return *this;
}

/*!
\brief Destructive subtraction operator.
*/
Matrix& Matrix::operator-=(const Matrix& u)
{
  for (int i=0;i<9;i++)
  {
    r[i]-=u.r[i];
  }
  return *this;
}

/*!
\brief Destructive multiplication operator.
*/
Matrix& Matrix::operator*=(double a)
{
  for (int i=0;i<9;i++)
  {
    r[i]*=a;
  }
  return *this;
}

/*!
\brief Destructive division operator.
*/
Matrix& Matrix::operator/=(double a)
{
  for (int i=0;i<9;i++)
  {
    r[i]/=a;
  }
  return *this;
}


/*!
\brief Compute the adjoint of the matrix.
*/
Matrix Matrix::Adjoint() const
{
  return Matrix(
    r[4]*r[8]-r[7]*r[5],
    -(r[3]*r[8]-r[6]*r[5]),
    r[3]*r[7]-r[6]*r[4],

    -(r[1]*r[8]-r[7]*r[2]),
    r[0]*r[8]-r[6]*r[2],
    -(r[0]*r[7]-r[6]*r[1]),

    r[1]*r[5]-r[4]*r[2],
    -(r[0]*r[5]-r[3]*r[2]),
    r[0]*r[4]-r[3]*r[1]);
}

/*!
\brief Overloaded.
\param s Stream.
\param matrix The matrix.
*/
ostream& operator<<(ostream& s, const Matrix& matrix)
{
  s<<"Matrix(";
  for (int i=0;i<3;i++)
  {
    for (int j=0;j<3;j++)
    {
      s<<matrix(i,j);
      if (i*3+j!=8)
      {
        s<<',';
      }
    }
  }
  s<<')';
  return s;
}

/*!
\brief Compute the covariance matrix of a point cloud.

This matrix is useful for computing the principal directions of a point of clouds.
The principal directions are defined as the eigen vectors of the matrix.

\sa EigenSolveSymmetric
\param p Array of vertices in space.
\param n Number of vertices.
*/
Matrix Matrix::Covariance(Vector *p,int n)
{
  Vector c=Vector(0.0);
  // Compute center of cloud
  for (int i=0;i<n;i++)
  {
    c+=p[i];
  }
  c/=n;

  Matrix C(0.0);
  for (int i=0;i<n;i++)
  {
    Vector e=p[i]-c;
    C[0]+=e[0]*e[0];
    C[1]+=e[0]*e[1];
    C[2]+=e[0]*e[2];
    //C[3]+=e[0]*e[1];
    C[4]+=e[1]*e[1];
    C[5]+=e[1]*e[2];
    //C[6]+=e[0]*e[2];
    //C[7]+=e[1]*e[2];
    C[8]+=e[2]*e[2];
  }
  C[3]=C[1];
  C[6]=C[2];
  C[7]=C[5];
  C/=n;
  return C; 
}

/*!
\brief Compute the angles of the rotation matrix.
*/
Vector Matrix::GetRotationAngles() const
{
  // Angles
  double x,y,z;

	const Matrix& m=*this;

  if (m(0,2) < +1)
  {
    if (m(0,2) > -1)
    {
      y = asin(m(0,2));
      x = atan2(-m(1,2),m(2,2));
      z = atan2(-m(0,1),m(0,0));
    }
    else // r02 = -1
    {
      // Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
      y = -Math::Pi/2;
      x = -atan2(m(1,0),m(1,1));
      z = 0.0;
    }
  }
  else // r02 = +1
  {
    // Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
    y = +Math::Pi/2;
    x = atan2(m(1,0),m(1,1));
    z = 0.0;
  }

  return Vector(x,y,z);
}
