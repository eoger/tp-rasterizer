// Fundamentals
// Copyright LIRIS/Geomod - Eric Galin

#ifndef __Mathematics__
#define __Mathematics__

#include <math.h>

//! Minimum of two integers.
inline int min(const int a, const int b)
{
  return (a<b?a:b);
}

//! Maximum of two integers.
inline int max(const int a, const int b)
{
  return (a>b?a:b);
}

//! Minimum of two doubles.
inline double min(const double& a, const double& b)
{
  return (a<b?a:b);
}

//! Maximum of two doubles.
inline double max(const double& a, const double& b)
{
  return (a>b?a:b);
}

inline double min(const double& a, const double& b, const double& c)
{
  return (a<b)?((a<c)?a:c):((b<c)?b:c);
}

inline double max(const double& a, const double& b, const double& c)
{
  return (a>b)?((a>c)?a:c):((b>c)?b:c);
}

// Math class for constants
class Math 
{
public:
  static const double Pi; //!< Pi.
  static const double HalfPi; //!< Half of pi.
  static const double e; //!< Exponential.
  static const double TwoPiOverThree; //!< 2/3 Pi.
  static const double FourPiOverThree; //!< 4/3 Pi.
public:
  static double Sqr(const double&);
  static double DegreeToRadian(const double&);
  static double Cycloidal(const double&);
  static double Triangle(const double&);
  static double Lerp(const double&,const double&,const double&);
  static int E(const double&);
};

//! Squares a double value.
inline double Math::Sqr(const double& x)
{
  return x*x;
}

/*! 
\brief Linear interpolation.

Returns (1-t)a+tb.

\param a,b Interpolated values.
\param t Interpolant.
*/
inline double Math::Lerp(const double& a,const double& b,const double& t)
{ 
  return a + t * (b - a); 
}

/*! 
\brief Convert degrees to randians.
\param x Angle in degrees.
*/
inline double Math::DegreeToRadian(const double& x)
{
	return x*Math::Pi/180.0;
}


//! Swap two reals.
inline void Swap(double& a,double& b)
{
  double t=a;
  a=b;
  b=t;
}

//! Modulo of two reals
inline double Modulo(const double& a,const double& b)
{
  int n=int(a/b);
  double c=a-n*b;
  if (c<0.0)
  {
    c+=b;
  }
  return c;
}

/*! 
\brief Compute the integer part of a real.

This function handles negative values differently by adding 1 to the result.
\param x %Real.
*/
inline int Math::E(const double& x) 
{
  return (x>0?int(x):int(x)-1);
}
#endif
