#pragma once

#include "evector.h"
#include "FrameBuffer.h"

struct Light
{
	Light(Vector _position, Color _color, double _ambientCoef) :
		pos(_position), color(_color), ambientCoef(_ambientCoef) {}
	Vector pos;
	Color color;
	double ambientCoef;
};
