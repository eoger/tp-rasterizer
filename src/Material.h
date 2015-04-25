#pragma once

#include "Color.h"

struct Material
{
	Material() {}
	Material(Color dc, double dp, Color sc, double sp, long sh) : diffuseColor(dc), diffusePower(dp), specularColor(sc), specularPower(sp), specularHardness(sh) {}
	static const Material Montain;
	static const Material Water;
	static const Material Snow;
	static const Material Grass;

	Color diffuseColor;
	double diffusePower;
	Color specularColor;
	double specularPower;
	long specularHardness;
};
