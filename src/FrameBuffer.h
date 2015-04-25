#ifndef _FRAME_BUFFER_H
#define _FRAME_BUFFER_H

#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <cfloat>

#include "Color.h"
#include "evector.h"
#include "matrix.h"
#include "Image_png.h"

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH  640

class Point3D : public Vector
{
	public:
	  Color color;
	  Point3D(Vector v, const Color & c) : Vector(v), color(c) {}
	  Point3D(double xx, double yy, double zz) : Vector(xx, yy, zz) {}
	  Point3D(double xx, double yy, double zz, const Color & c) : Vector(xx, yy, zz), color(c) {}
	  Point3D() : Vector() {}
	  ~Point3D() {}
	  friend ostream& operator<<(ostream& s, const Point3D& p) { s << "(" << p[0] << ',' << p[1] << ',' << p[2] << ')'; return s; }
	  Point3D& operator=(const Vector& v) {
		  (*this)[0] = v[0];
		  (*this)[1] = v[1];
		  (*this)[2] = v[2];
		  return *this;
	  }
};

struct PNGColor
{
	PNGColor() { r = g = b = 0; }
	PNGColor(Color c) : r(c[0] * 255.0), g(c[1] * 255.0), b(c[2] * 255.0) {}
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

#define ABS(x) ((x >= 0) ? x : -x)

class FrameBuffer {
   protected:
      // Simulated frame buffer
      Color *screen;//[SCREEN_HEIGHT][SCREEN_WIDTH];
	  double *zbuffer;

      long width;
      long height;


      // min X and max X for every horizontal line within the triangle
      long *contourX;//[SCREEN_HEIGHT][2];
      Color *contourXC;//[SCREEN_HEIGHT][2];

   public:
      FrameBuffer(long w,long h,Color bgColor)
      {
         width = w;
         height = h;
         screen = new Color[width*height];
		 fill_n(screen, width*height, bgColor);
		 zbuffer = new double[width*height];
		 std::fill_n(zbuffer, width*height, DBL_MAX);
         contourX = new long[height*2];
         contourXC = new Color[height*2];
      }

      ~FrameBuffer()
	  {
		 delete[] screen;
		 delete[] zbuffer;
         delete[] contourX;
         delete[] contourXC;
      }

	  Color BlendColor(const Color & c1, const Color & c2, double alpha)
	  {
		  return Color(c1[0] * alpha + c2[0] * (1.0 - alpha), c1[1] * alpha + c2[1] * (1.0 - alpha), c1[2] * alpha + c2[2] * (1.0 - alpha));
	  }

	  bool TestSetZValue(long x, long y, double z)
	  {
		  if ((x < 0) || (x >= width) ||
			  (y < 0) || (y >= height))
		  {
			  return false;
		  }

		  if (z <= zbuffer[y*width + x])
		  {
			  zbuffer[y*width + x] = z;
			  return true;
		  }

		  return false;
	  }

      void SetPixel(long x, long y, const Color & color)
	  {
		  if ((x < 0) || (x >= width) ||
			  (y < 0) || (y >= height))
		  {
			  return;
		  }

		  screen[y*width + x] = color;
      }

      // Scans a side of a triangle setting min X and max X in ContourX[][]
      // (using the Bresenham's line drawing algorithm).
      void ScanLine(long x1, long y1, long x2, long y2, const Color & c1, const Color & c2)
      {
        long sx, sy, dx1, dy1, dx2, dy2, x, y, m, n, k, cnt;

        sx = x2 - x1;
        sy = y2 - y1;

        if (sx > 0) dx1 = 1;
        else if (sx < 0) dx1 = -1;
        else dx1 = 0;

        if (sy > 0) dy1 = 1;
        else if (sy < 0) dy1 = -1;
        else dy1 = 0;

        m = ABS(sx);
        n = ABS(sy);
        dx2 = dx1;
        dy2 = 0;

        if (m < n)
        {
          m = ABS(sy);
          n = ABS(sx);
          dx2 = 0;
          dy2 = dy1;
        }

        x = x1; y = y1;
        cnt = m + 1;
        k = m / 2;

        int total = cnt;

        Color color;

        while (cnt--)
        {
          color = BlendColor(c1,c2,(double)cnt/(double)total); 
          if ((y >= 0) && (y < height))
          {
            if (x < contourX[y*2]) 
            {
               contourX[y*2] = x;
               contourXC[y*2] = color;
            }
            if (x > contourX[y*2+1]) 
            {
               contourX[y*2+1] = x;
               contourXC[y*2+1] = color;
            }
          }

          k += n;
          if (k < m)
          {
            x += dx2;
            y += dy2;
          }
          else
          {
            k -= m;
            x += dx1;
            y += dy1;
          }
        }
      }

      void DrawTriangle(Point3D p0, Point3D p1, Point3D p2)
      {
        int y;

        for (y = 0; y < SCREEN_HEIGHT; y++)
        {
          contourX[y*2] = LONG_MAX; // min X
          contourX[y*2+1] = LONG_MIN; // max X
        }

        ScanLine(p0[0], p0[1], p1[0], p1[1], p0.color, p1.color );
        ScanLine(p1[0], p1[1], p2[0], p2[1], p1.color, p2.color );
        ScanLine(p2[0], p2[1], p0[0], p0[1], p2.color, p0.color );

        for (y = 0; y < height; y++)
        {
          if (contourX[y*2+1] >= contourX[y*2])
          {
            long x = contourX[y*2];
            long len = 1 + contourX[y*2+1] - contourX[y*2];

            long total = len;
            Color color;

            // Can draw a horizontal line instead of individual pixels here
            while (len--)
            {
              color = BlendColor(contourXC[y*2],contourXC[y*2+1],(double)len/(double)total);


			  // ## ZBUFFER ##

			  // On va interpoler Z pour le point à dessiner avec une interpolation bilinéaire

			  Vector polycoefs = Vector(p0[2], p1[2], p2[2]) * Inverse(Matrix(p0[0], p0[1], 1, p1[0], p1[1], 1, p2[0], p2[1], 1));
			  double z = polycoefs[0] * x + polycoefs[1] * y + polycoefs[2];
			  
			  if (TestSetZValue(x, y, z))
			  {
				SetPixel(x, y, color);
			  }
			  x++;
            }
          }
        }
      }

      bool SavePng(const char * filename)
      {
		  PNGColor * rgb255data = new PNGColor[width * height];
		  for (int i = 0; i < width * height; i++)
		  {
			  rgb255data[i] = screen[i];
		  }
		  bool result = Image::savePng(filename, (const unsigned char *)rgb255data, width, height, 3, 1);
		  delete[] rgb255data;
		  return result;
      }
};

#endif
