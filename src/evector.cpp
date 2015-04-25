// Vector  
// Copyright LIRIS/Geomod - Eric Galin

// Self include
#include "evector.h"

#include <iostream>
using namespace std;

/*!
\class Vector evector.h
\brief Vectors in real space.

Most binary operators have been overloaded as expected,
destructive operators, such as addition and subtraction 
have been implemented and behave as one could expect. 

<P><I>How do I compute the cross product of two vectors?</I>    
<BR>Simply use the overloaded Vector::operator/, for instance 
\code 
Vector c=a/b;
\endcode 
computes the cross product of a and b.
<P><I>How do I compute the sine of the angle between two vectors?</I>    
<BR>Simply use the Sine(const Vector&,const Vector&) function, which internally computes the norm of the cross product divided by the norm of the argument vectors.
\code 
double s=Sine(a,b); // Equivalent to Norm(a/b)/(Norm(a)*Norm(b));
\endcode 
<P><I>How can I get access to the x, y and z components of a vector?</I>    
<BR>Use v[0], v[1] and v[2] to get access to the x, y and z components of a vector v respectively.
<P><I>How do I compute the normal of a triangle?</I>    
<BR>Let a,b,c the vertices of the triangle, simply compute the cross product 
\code 
Vector n=(a-b)/(a-c);
\endcode 
or use the member function of the Triangle class: 
\code 
Vector n=Triangle(a,b,c).Normal();
\endcode

\ingroup Math
*/

/*!
\brief Check if three vectors are coplanar.

Simply compute the cross product of a and b, and the dot product with c.
Compare the result with a given tolerance.

\param a,b,c Vectors.
\param epsilon Tolerance parameter.
*/
int Coplanar(const Vector& a,const Vector& b,const Vector& c,const double& epsilon)
{
  double s=fabs((a/b)*c)/(Norm(a)*Norm(b)*Norm(c));
  return (s<epsilon);
}

/*!
\brief Test if two vectors are almost equal.

This function computes the difference between the two argument vectors, and then the norm infinity of this difference and check the result againt the epsilon threshold value.
This is a convenience function which is the same as:
\code
Vector a,b; // Two vectors
double e; // Epsilon value
bool e=NormInfinity(Abs(b-a))<e?true:false;
\endcode
*/
bool Equal(const Vector& a,const Vector& b,const double& epsilon)
{
  Vector ab=Abs(b-a);
  if(ab[0]>epsilon || ab[1]>epsilon || ab[2]>epsilon)
    return false;
  return true;
}

/*!
\brief Normalize a vector, computing the inverse of its norm and scaling 
the components. 

This function does not check if the vector is null,
which might resulting in errors.
*/
void Normalize(Vector& u)
{
  u*=1.0/Norm(u);
}

/*!
\brief Returns the positive sine of two vectors. Basically computes the 
cross product of the vectors and normalizes the result.
\param u, v Vectors.
*/
double Sine(const Vector& u,const Vector& v)
{
  return Norm(u/v)/sqrt((u*u)*(v*v));
}

/*!
\brief Returns the positive cosine of two vectors. Basically computes the 
dot product of the normalized vectors.
*/
double Cosine(const Vector& u,const Vector& v)
{
  return (u*v)/sqrt((u*u)*(v*v));
}

/*!
\brief Returns alignment boolean. 
Basically computes the cosine of the two vectors, and checks for unity.
\param u, v Vectors.
*/
int Aligned(const Vector& u,const Vector& v)
{
  double c=Cosine(u,v);
  c*=c;
  return (c>(1.0-0.0001));
}

/*!
\brief Swap two vectors.
\param a, b Vectors.
*/
void Swap(Vector& a,Vector& b)
{
  Vector t=a;
  a=b;
  b=t;
}


/*!
\brief Checks if four points are coplanar.
*/
int Coplanar(const Vector& t,const Vector& u,const Vector& v,const Vector& w)
{
  return Coplanar(u-t,v-t,w-t, 1e-6);
}

/*!
\brief Returns a vector orthogonal to the argument vector.

The returned orthogonal vector is not computed randomly. 
First, we find the two coordinates of the argument vector with
maximum absolute value. The orthogonal vector is defined by
swapping those two coordinates and changing one sign, whereas
the third coordinate is set to 0. 

The returned orthogonal vector lies in the plane orthogonal
to the first vector.

\param u Argument vector.
*/
Vector Orthogonal(const Vector& u)
{  
  Vector a=Abs(u);
  int i=0;
  int j=1;
  if (a[0]>a[1])
  {
    if (a[2]>a[1])
    {
      j=2;
    }
  }
  else
  {
    i=1;
    j=2;
    if (a[0]>a[2])
    {
      j=0;
    }
  }
  a=Vector(0.0);
  a[i]=u[j];
  a[j]=-u[i];
  return a;
}

/*!
\brief Overloaded output-stream operator.
\param u Vector.
\param s Stream.
*/
ostream& operator<<(ostream& s, const Vector& u)
{
  s<<"Vector("<<u.c[0]<<','<<u.c[1]<<','<<u.c[2]<<')';
  return s;
}

/*!
\brief Given a vector, creates two vectors xand y that form an orthogonal basis.

This algorithm pickes the minor axis in order to reduce numerical instability
\param n Argument vector.
\param x, y Returned vectors such that (x,y,n) form an orthonormal basis (provided n is normalized).
*/
void OrthonormalStable(const Vector& n, Vector& x, Vector& y)
{
  double min=fabs(n[0]);
  int k=0;

  // Find the minor axis of the ray
  for (int i=1; i < 3; i++)
  {
    if (fabs(n[i]) < min)
    {
      min=fabs(n[i]);
      k=i;
    }
  }

  if (k == 0)
  {
    x=Normalized(Vector(0,-n[2],n[1]));
    y=Normalized(x/n);
  }
  else if (k == 1)
  {
    x=Normalized(Vector(-n[2],0,n[0]));
    y=Normalized(x/n);
  }
  else if (k == 2)
  {
    x=Normalized(Vector(-n[1],n[0],0));
    y=Normalized(x/n);
  }
}


