#pragma once
#include <cassert>
#include "evector.h"
class Color : public Vector
{
public:
	Color() : Vector(0, 0, 0) {}
	Color(const Vector& v) : Vector(v) {}
	Color(double rr, double gg, double bb) : Vector(rr, gg, bb) {
		//assert(rr <= 1.0 && gg <= 1.0 && bb <= 1.0);
	}

	static Color RGB255(unsigned char r, unsigned char g, unsigned char b)
	{
		return Color(double(r) / 255.0, double(g) / 255.0, double(b) / 255.0);
	}

	friend Color operator* (const Color& b, const double& a)
	{
		return Color(a * b[0], a * b[1], a * b[2]);
	}

	friend Color operator* (const double& a, const Color& b)
	{
		return Color(a * b[0], a * b[1], a * b[2]);
	}

	friend Color operator* (const Color& a, const Color& b)
	{
		return Color(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
	}

	friend Color operator+ (const Color& a, const Color& b)
	{
		return Color(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
	}

	friend Color pow(const Color& c, double a)
	{
		return Color(pow(c[0], a), pow(c[1], a), pow(c[2], a));
	}
};