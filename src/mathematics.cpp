// Mathematics
// Copyright LIRIS/Geomod - Eric Galin

#include "mathematics.h"

/*! 
\defgroup Math Core math classes

\brief Core math classes include several classes such as Vector, Quadric, Cubic and higher order 
polynomials and many others that are useful in many graphic applications.

\changed 12.12.23
*/

/*!
\class Math mathematics.h
\brief Core class implementing some useful functions and constants.

<P><I>How do I use the constant Pi?</I>    
<BR>Simply use the static constant Math::Pi as follows: 
\code 
double v=4.0*Math::Pi*r*r*r/3.0; // Volume of a sphere.
\endcode 

\ingroup Math
\changed 13.01.24
*/
const double Math::Pi=3.14159265358979323846;

const double Math::HalfPi=Math::Pi/2.0;

const double Math::e=2.7182818284590452354;

const double Math::TwoPiOverThree=2.0943951023931954923084;

const double Math::FourPiOverThree=4.1887902047863909846168;

/*! 
\brief Sine wave over unit interval.
\param x Input variable.
*/
double Math::Cycloidal(const double& x)
{
  return sin(x-floor(x)*2.0*Math::Pi);
}

/*! 
\brief Triangle wave over unit interval.
\param x Input variable.
*/
double Math::Triangle(const double& x)
{
  double offset;

  if (x>=0.0) 
  {
    offset=x-floor(x);
  }
  else
  {
    offset=x-(-1.0-floor(fabs(x)));
  }
  if (offset>=0.5) 
  {
    return (2.0*(1.0-offset));
  }
  else 
  {
    return (2.0*offset);
  }
}
