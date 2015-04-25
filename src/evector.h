// Vector  
// Copyright LIRIS/Geomod - Eric Galin

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifndef __Vector__
#define __Vector__

#include <math.h>
#include <iostream>
using namespace std;

// Mathematics fundamentals
#include "mathematics.h"

// Class
class Vector
{
protected:
  double c[3]; //!< Vector components.
public:
  //! Empty 
  Vector() { }
  //! Create a vector with the same coordinates.
  Vector(const double& a) { c[0]=c[1]=c[2]=a; }
  //! Create a vector with argument coordinates.
  Vector(const double& a, const double& b, const double& c) { Vector::c[0]=a; Vector::c[1]=b; Vector::c[2]=c; }
  //! Create a vector with argument coordinates in an array
  Vector(const double a[3]) { c[0]=a[0]; c[1]=a[1]; c[2]=a[2]; }

  // Access members
  double& operator[] (int);
  double operator[] (int) const;

  // Unary operators
  Vector operator+ () const;
  Vector operator- () const;

  // Assignment operators
  Vector& operator+= (const Vector&);
  Vector& operator-= (const Vector&);
  Vector& operator*= (const Vector&);
  Vector& operator/= (const Vector&);
  Vector& operator*= (const double&);
  Vector& operator/= (const double&);

  // Binary operators
  friend int operator> (const Vector&, const Vector&);
  friend int operator< (const Vector&, const Vector&);

  friend int operator>= (const Vector&, const Vector&);
  friend int operator<= (const Vector&, const Vector&);

  // Binary operators
  friend Vector operator+ (const Vector&, const Vector&);
  friend Vector operator- (const Vector&, const Vector&);

  friend double operator* (const Vector&, const Vector&);

  friend Vector operator* (const Vector&, double);
  friend Vector operator* (const double&, const Vector&);
  friend Vector operator/ (const Vector&, double);

  friend Vector operator/ (const Vector&, const Vector&);

  // Boolean functions
  friend int operator==(const Vector&,const Vector&);
  friend int operator!=(const Vector&,const Vector&);

  // Norm
  friend double Norm(const Vector&);
  friend double SquaredNorm(const Vector&);
  friend double NormInfinity(const Vector&);
  friend void Normalize(Vector&);
  friend Vector Normalized(const Vector&);

  friend bool Equal(const Vector&,const Vector&,const double& );

  // High level functions
  friend double Sine(const Vector&,const Vector&);
  friend double Cosine(const Vector&,const Vector&);

  // Compare functions
  friend Vector min(const Vector&,const Vector&);
  friend Vector max(const Vector&,const Vector&);

  // Abs
  friend Vector Abs(const Vector&);

  // Modulo
  friend Vector Modulo(const Vector&,const Vector&);

  // Orthogonal
  friend Vector Orthogonal(const Vector&);
  friend void OrthonormalStable(const Vector&,Vector&,Vector&);

  // Swap
  friend void Swap(Vector&,Vector&);

  friend int Aligned(const Vector&,const Vector&);
  friend int Coplanar(const Vector&,const Vector&,const Vector&,const double&);
  friend int Coplanar(const Vector&,const Vector&,const Vector&,const Vector&);
  friend Vector Clamp(const Vector&,const Vector&,const Vector&);
  friend Vector Lerp(const Vector&,const Vector&,const double&);

  // Scale
  Vector Scale(const Vector&) const;

  friend ostream& operator<<(ostream&,const Vector&);

  static Vector Polar(const double&,const double&);
};

//! Gets the i<SUP>th</SUP> coordinate of vector.
inline double& Vector::operator[] (int i) 
{
  return c[i];
}

//! Returns the i<SUP>th</SUP> coordinate of vector.
inline double Vector::operator[] (int i) const 
{
  return c[i];
}

// Unary operators

//! Overloaded.
inline Vector Vector::operator+ () const
{
  return *this;
}

//! Overloaded.
inline Vector Vector::operator- () const
{
  return Vector(-c[0],-c[1],-c[2]);
}

// Assignment unary operators

//! Destructive addition.
inline Vector& Vector::operator+= (const Vector& u)
{
  c[0]+=u.c[0]; c[1]+=u.c[1]; c[2]+=u.c[2];
  return *this;
}

//! Destructive subtraction.
inline Vector& Vector::operator-= (const Vector& u)
{
  c[0]-=u.c[0]; c[1]-=u.c[1]; c[2]-=u.c[2];
  return *this;
}

//! Destructive scalar multiply.
inline Vector& Vector::operator*= (const double& a)
{
  c[0]*=a; c[1]*=a; c[2]*=a;
  return *this;
}

/*! 
\brief Destructive scalar multiply by a vector.

This function scales the components one by one.
*/
inline Vector Vector::Scale(const Vector& a) const
{
  return Vector(c[0]*a[0],c[1]*a[1],c[2]*a[2]);
}

//! Destructive division by a scalar.
inline Vector& Vector::operator/= (const double& a)
{
  c[0]/=a; c[1]/=a; c[2]/=a;
  return *this;
}

//! Destructively scale a vector by another vector.
inline Vector& Vector::operator*= (const Vector& u)
{
  c[0]*=u.c[0]; c[1]*=u.c[1]; c[2]*=u.c[2];
  return *this;
}

//! Destructively divide the components of a vector by another vector.
inline Vector& Vector::operator/= (const Vector& u)
{
  c[0]/=u.c[0]; c[1]/=u.c[1]; c[2]/=u.c[2];
  return *this;
}

//! Compare two vectors.
inline int operator> (const Vector& u, const Vector& v)
{
  return ((u.c[0]>v.c[0]) && (u.c[1]>v.c[1]) && (u.c[2]>v.c[2]));
}

//! Compare two vectors.
inline int operator< (const Vector& u, const Vector& v)
{
  return ((u.c[0]<v.c[0]) && (u.c[1]<v.c[1]) && (u.c[2]<v.c[2]));
}

//! Overloaded
inline int operator>= (const Vector& u, const Vector& v)
{
  return ((u.c[0]>=v.c[0]) && (u.c[1]>=v.c[1]) && (u.c[2]>=v.c[2]));
}

//! Overloaded
inline int operator<= (const Vector& u, const Vector& v)
{
  return ((u.c[0]<=v.c[0]) && (u.c[1]<=v.c[1]) && (u.c[2]<=v.c[2]));
}

//! Adds up two vectors.
inline Vector operator+ (const Vector& u, const Vector& v)
{
  return Vector(u.c[0]+v.c[0],u.c[1]+v.c[1],u.c[2]+v.c[2]);
}

//! Difference between two vectors.
inline Vector operator- (const Vector& u, const Vector& v)
{
  return Vector(u.c[0]-v.c[0],u.c[1]-v.c[1],u.c[2]-v.c[2]);
}

//! Scalar product.
inline double operator* (const Vector& u, const Vector& v)
{
  return (u.c[0]*v.c[0]+u.c[1]*v.c[1]+u.c[2]*v.c[2]);
}

//! Right multiply by a scalar.
inline Vector operator* (const Vector& u,double a)
{
  return Vector(u.c[0]*a,u.c[1]*a,u.c[2]*a);
}

//! Left multiply by a scalar.
inline Vector operator* (const double& a, const Vector& v)
{
  return v*a;
}

//! Cross product.
inline Vector operator/ (const Vector& u, const Vector& v)
{
  return Vector(u.c[1]*v.c[2]-u.c[2]*v.c[1],u.c[2]*v.c[0]-u.c[0]*v.c[2],u.c[0]*v.c[1]-u.c[1]*v.c[0]);
}

//! Left multiply by a scalar
inline Vector operator/ (const Vector& u, double a)
{
  return Vector(u.c[0]/a,u.c[1]/a,u.c[2]/a);
}

// Boolean functions

//! Strong equality test.
inline int operator== (const Vector& u,const Vector& v)
{
  return ((u.c[0]==v.c[0])&&(u.c[1]==v.c[1])&&(u.c[2]==v.c[2]));
}

//! Strong difference test.
inline int operator!= (const Vector& u,const Vector& v)
{
  return (!(u==v));
}

/*!
\brief Compute the Euclidean norm of a vector.
This function involves a square root computation, it is often more efficient to rely on
the squared norm of a vector instead. \sa SquaredNorm
*/
inline double Norm(const Vector& u)
{
  return sqrt(u.c[0]*u.c[0]+u.c[1]*u.c[1]+u.c[2]*u.c[2]);
}

/*!
\brief Compute the squared Euclidean norm of a vector.
*/
inline double SquaredNorm(const Vector& u)
{
  return (u.c[0]*u.c[0]+u.c[1]*u.c[1]+u.c[2]*u.c[2]);
}

/*!
\brief Return a Normalized a vector, computing the inverse of its 
norm and scaling the components. 
This function does not check if 
the vector is null, which might resulting in errors.
*/
inline Vector Normalized(const Vector& u)
{
  return u*(1.0/Norm(u));
}

/*!
\brief Compute the infinity norm of a vector.
*/
inline double NormInfinity(const Vector& u)
{
  return max(fabs(u.c[0]),fabs(u.c[1]),fabs(u.c[2]));
}

/*!
\brief Computes the absolute value of a vector.
*/
inline Vector Abs(const Vector& u)
{
  return Vector(u[0]>0.0?u[0]:-u[0],u[1]>0.0?u[1]:-u[1],u[2]>0.0?u[2]:-u[2]);
}

/*!
\brief Return a vector with coordinates set to the minimum coordinates
of the two argument vectors.
*/
inline Vector min(const Vector& a,const Vector& b)
{
  return Vector(a[0]<b[0]?a[0]:b[0],a[1]<b[1]?a[1]:b[1],a[2]<b[2]?a[2]:b[2]);
}

/*!
\brief Return a vector with coordinates set to the maximum coordinates
of the two argument vectors.
*/
inline Vector max(const Vector& a,const Vector& b)
{
  return Vector(a[0]>b[0]?a[0]:b[0],a[1]>b[1]?a[1]:b[1],a[2]>b[2]?a[2]:b[2]);
}

/*! 
\brief Clamp a double value between two bounds.
\param x Input value.
\param a, b Lower and upper bounds
*/
inline double Clamp(const double& x, const double& a =0.0, const double& b =1.0)
{
  return (x < a ? a : (x > b ? b : x));
}

//! Clamps an integer value between two bounds.
inline double Clamp(int x, int a, int b)
{
  return (x < a ? a : (x > b ? b : x));
}

//! Clamps an float value between two bounds.
inline float Clamp(float x, float a, float b)
{
  return (x < a ? a : (x > b ? b : x));
}

/*! 
\brief Clamp a vector between two bounds.
\param x Input vector
\param a, b %Vector bounds.
*/
inline Vector Clamp(const Vector& x, const Vector& a, const Vector& b)
{
  return Vector(Clamp(x[0],a[0],b[0]),Clamp(x[1],a[1],b[1]),Clamp(x[2],a[2],b[2]));
}

/*! 
\brief Linear interpolation between two vectors.
\param a,b Interpolated points.
\param t Interpolant.
*/
inline Vector Lerp(const Vector& a,const Vector& b,const double& t)
{ 
  return a + t * (b - a); 
}

/*!
\brief Creates a vector given polar coordinates.
\param t Theta.
\param p Phi.
*/
inline Vector Vector::Polar(const double& t,const double& p)
{
  return Vector( sin(p)*sin(t), cos(p)*sin(t), cos(t) );
}

//! Modulo of two Vectors.
inline Vector Modulo(const Vector& a,const Vector& b)
{
  return Vector(Modulo(a[0],b[0]),Modulo(a[1],b[1]),Modulo(a[2],b[2]));
}

#endif

