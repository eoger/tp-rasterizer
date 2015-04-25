// Matrix 
// Copyright LIRIS/Geomod - Eric Galin

#ifndef __Matrix__
#define __Matrix__

// Include Vector class
#include "evector.h"

class Matrix {
protected:
  double r[9];  //!< The array storing the coefficients of the matrix.
public:
  //! Empty.
  Matrix() {}
  Matrix(const double&);
  Matrix(const Vector&);
  Matrix(const Vector&,const Vector&,const Vector&);
  Matrix(const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&);

  static const Matrix Zero;
  static const Matrix Identity;

  // Functions to access Matrix class components  
  double& operator[] (int);
  double operator[] (int) const;

  double& operator() (int,int);
  double operator() (int,int) const;

  // Unary operators
  Matrix operator+ () const { return *this; }
  Matrix operator- () const;

  // Assignment operators
  Matrix& operator+= (const Matrix&);
  Matrix& operator-= (const Matrix&);
  Matrix& operator*= (const Matrix&);
  Matrix& operator*= (double);
  Matrix& operator/= (double);

  // Binary operators
  friend Matrix operator+ (const Matrix&, const Matrix&);
  friend Matrix operator- (const Matrix&, const Matrix&);  
  friend Matrix operator* (const Matrix&, const Matrix&);

  friend Vector operator* (const Matrix&, const Vector&);
  friend Vector operator* (const Vector&, const Matrix&);

  friend Matrix operator* (const Matrix&, const double&);
  friend Matrix operator* (const double&, const Matrix&);

  friend Matrix operator/ (const Matrix&, const double&);
  friend Matrix operator/ (const double&, const Matrix&);

  static Matrix Rotation(const Vector&);
  static Matrix Rotation(const Vector&,const double&);
  static Matrix Rotation(const Vector&,const Vector&);

  static Matrix RotationX(const double&);
  static Matrix RotationY(const double&);
  static Matrix RotationZ(const double&);

  Vector GetRotationAngles() const;
  static Matrix Covariance(Vector*,int);

  // Self
  void Id();
  void Null();
  friend ostream& operator<<(ostream&, const Matrix&);

  // Transpose
  Matrix T() const;

  // Adjoint
  Matrix Adjoint() const; 

  // Determinant
  double Determinant() const;

  friend Matrix Inverse(const Matrix&); 
  friend double Trace(const Matrix&);

  // Algebra
  double SpectralNorm() const;

  // Singular values methods
  void SingularValueDecomposition(Matrix&,Vector&,Matrix&) const;

  void QDU(Matrix&,Vector&,Vector&) const;

  void ExtractAngleAxis(double&, Vector&) const;

  // Eigensolver, matrix must be symmetric
  void EigenSolveSymmetric (double [3], Vector [3]) const;
  Vector Eigen() const;

  void Tridiagonal(double [3], double [2]);
  int QLAlgorithm(double [3], double [3]);
private:
  double MCR2 (double [3]) const;
  void Bidiagonalize(Matrix& L, Matrix& R);
  void GolubKahanStep(Matrix& L, Matrix& R);
private:
  static double epsilon; //!< Epsilon value used to check angles of a rotation matrix, and in the implementation of some algebra methods.
};

//! Defines a null matrix.
inline void Matrix::Null()
{
  r[0]=r[4]=r[8]=r[1]=r[2]=r[3]=r[5]=r[6]=r[7]=0.0;
}

//! Direct access to the array of the matrix.
inline  double& Matrix::operator[] (int i) 
{ 
  return r[i]; 
}

//! Overloaded.
inline double Matrix::operator[] (int i) const 
{
  return r[i]; 
}

/*!
\brief Get element (i,j) of the matrix.
\param i Row.
\param j Column.
*/
inline  double& Matrix::operator() (int i,int j) 
{  
  return r[i+j+j+j];
}

//! Overloaded.
inline double Matrix::operator() (int i,int j) const 
{ 
  return r[i+j+j+j]; 
}


//! Creates a matrix with a set of double values.
inline Matrix::Matrix(const double& a00,const double& a01,const double& a02,const double& a10,const double& a11,const double& a12,const double& a20,const double& a21,const double& a22)
{
  r[0]=a00;
  r[1]=a01;
  r[2]=a02;

  r[3]=a10;
  r[4]=a11;
  r[5]=a12;

  r[6]=a20;
  r[7]=a21;
  r[8]=a22;
}

/*! 
\brief Set the matrix to identity.
*/
inline void Matrix::Id()
{
  r[1]=r[2]=r[3]=r[5]=r[6]=r[7]=0.0;
  r[0]=r[4]=r[8]=1.0;
}

/*! 
\brief Creates a matrix with the same diagonal value.
\param a Diagonal value.
*/
inline Matrix::Matrix(const double& a)
{
  r[1]=r[2]=r[3]=r[5]=r[6]=r[7]=0.0;
  r[0]=r[4]=r[8]=a;
}

/*! 
\brief Create a diagonal matrix with diagonal terms set to the vector entries.
\param a %Vector of diagonal values
*/
inline Matrix::Matrix(const Vector& a)
{
  r[1]=r[2]=r[3]=r[5]=r[6]=r[7]=0.0;
  r[0]=a[0];
  r[4]=a[1];
  r[8]=a[2];
}

/*! 
\brief Creates a column vector matrix.
\param a,b,c Column vectors.
*/
inline Matrix::Matrix(const Vector& a,const Vector& b,const Vector& c)
{
  r[0]=a[0];
  r[1]=a[1];
  r[2]=a[2];

  r[3]=b[0];
  r[4]=b[1];
  r[5]=b[2];

  r[6]=c[0];
  r[7]=c[1];
  r[8]=c[2];
}

//! Transpose a matrix.
inline Matrix T(const Matrix& r) 
{
  return Matrix(r[0],r[3],r[6],r[1],r[4],r[7],r[2],r[5],r[8]);
}

//! Transpose a matrix.
inline Matrix Matrix::T() const
{
  return Matrix(r[0],r[3],r[6],r[1],r[4],r[7],r[2],r[5],r[8]);
}

/*!
\brief Computes the determinant of the matrix.
*/
inline double Matrix::Determinant() const
{
  return r[0]*r[4]*r[8]+r[1]*r[5]*r[6]+r[2]*r[3]*r[7]-r[2]*r[4]*r[6]-r[1]*r[3]*r[8]-r[0]*r[5]*r[7];
}

//! Returns the opposite of a matrix -A.
inline Matrix Matrix::operator-() const
{
  return Matrix(-r[0],-r[1],-r[2],-r[3],-r[4],-r[5],-r[6],-r[7],-r[8]);
}

//! Compute the trace (sum of diagonal terms) of a matrix.
inline double Trace(const Matrix& a) 
{
  return a[0]+a[4]+a[8];
}

//! Right multiply by a vector, computations have been inlined out of efficiency. 
inline Vector operator*(const Matrix& A, const Vector& v)
{
  return Vector(v[0]*A[0]+v[1]*A[3]+v[2]*A[6],v[0]*A[1]+v[1]*A[4]+v[2]*A[7],v[0]*A[2]+v[1]*A[5]+v[2]*A[8]);
}

//! Left multiply by a vector, computations have been inlined out of efficiency. 
inline Vector operator*(const Vector& v,const Matrix& A)
{
  return Vector (v[0]*A[0]+v[1]*A[1]+v[2]*A[2],v[0]*A[3]+v[1]*A[4]+v[2]*A[5],v[0]*A[6]+v[1]*A[7]+v[2]*A[8]);  
}

//! Overloaded. The loop has been coded inline out of efficiency.
inline Matrix operator-(const Matrix& u, const Matrix& v)
{
  return Matrix(u[0]-v[0],u[1]-v[1],u[2]-v[2],u[3]-v[3],u[4]-v[4],u[5]-v[5],u[6]-v[6],u[7]-v[7],u[8]-v[8]);
}

//! Overloaded. The loop has been coded inline out of efficiency.
inline Matrix operator+(const Matrix& u, const Matrix& v)
{
  return Matrix(u[0]+v[0],u[1]+v[1],u[2]+v[2],u[3]+v[3],u[4]+v[4],u[5]+v[5],u[6]+v[6],u[7]+v[7],u[8]+v[8]);
}

//! Right multiply by a double. 
inline Matrix operator*(const Matrix& u, const double& a)
{
  return Matrix(a*u[0],a*u[1],a*u[2],a*u[3],a*u[4],a*u[5],a*u[6],a*u[7],a*u[8]);
}

//! Left multiply by a double, computations have been inlined out of efficiency. 
inline Matrix operator*(const double& a,const Matrix& u)
{
  return Matrix(a*u[0],a*u[1],a*u[2],a*u[3],a*u[4],a*u[5],a*u[6],a*u[7],a*u[8]);
}

//! Right divide by a double. The loop has been coded inline out of efficiency.
inline Matrix operator/(const Matrix& u, const double& a)
{
  return Matrix(u[0]/a,u[1]/a,u[2]/a,u[3]/a,u[4]/a,u[5]/a,u[6]/a,u[7]/a,u[8]/a);
}

// Extended matrix
class Matrix4 { 
protected:
  double r[16];
public:
  //! Empty
  Matrix4() { }
  Matrix4(const double&);
  Matrix4(const Vector&);
  Matrix4(const Matrix&);
  Matrix4(const Matrix&,const Vector&);
  Matrix4(const Matrix&,const Vector&,const Vector&);
  Matrix4(const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&,const double&);

  static const Matrix4 Zero;//!< Null matrix.
  static const Matrix4 Identity;//!< Identity matrix.

  // Functions to access class components
  double& operator[] (int i) { return r[i]; }
  const double& operator[] (int i) const { return r[i]; }

  double& operator() (int i,int j) { return r[i+(j<<2)]; }
  const double& operator() (int i,int j) const { return r[i+(j<<2)]; }

  // Unary operators
  Matrix4 operator+ () const { return *this; }
  Matrix4 operator- () const;

  // Assignment operators
  Matrix4& operator+= (const Matrix4&);
  Matrix4& operator-= (const Matrix4&);
  Matrix4& operator*= (const Matrix4&);
  Matrix4& operator*= (double);
  Matrix4& operator/= (double);

  // Binary operators
  friend Matrix4 operator+ (const Matrix4&, const Matrix4&);
  friend Matrix4 operator- (const Matrix4&, const Matrix4&);  
  friend Matrix4 operator* (const Matrix4&, const Matrix4&);

  friend Vector operator* (const Matrix4&, const Vector&);

  friend double Det(const Matrix4&);

  // Self
  void Id();
  void Null();

  friend Matrix4 T(const Matrix4&);
  friend Matrix4 Inverse(const Matrix4&);

  static Matrix4 Rotate(const Vector&, const double&);
  static Matrix4 Rotate(const Vector&);
  static Matrix4 Scale(const Vector&);
  static Matrix4 Translate(const Vector&);
  static Matrix4 Shear(const Vector&);

  friend ostream& operator<<(ostream&, const Matrix4&);
protected:
  static double iepsilon; //!< Epsilon value used to check if the determinant of a matrix is null.
};
/*!
\brief Create a matrix given its 16 coefficients.
*/
inline Matrix4::Matrix4(const double& a0,const double& a1,const double& a2,const double& a3,const double& a4,const double& a5,const double& a6,const double& a7,const double& a8,const double& a9,const double& a10,const double& a11,const double& a12,const double& a13,const double& a14,const double& a15)
{
  r[0]=a0;
  r[4]=a1;
  r[8]=a2;
  r[12]=a3;

  r[1]=a4;
  r[5]=a5;
  r[9]=a6;
  r[13]=a7;

  r[2]=a8;
  r[6]=a9;
  r[10]=a10;
  r[14]=a11;

  r[3]=a12;
  r[7]=a13;
  r[11]=a14;
  r[15]=a15;
}

#endif
