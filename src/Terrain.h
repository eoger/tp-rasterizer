#pragma once

#include <iostream>
#include "VertexBuffer.h"

class Terrain
{

protected:
	long height;
	long width;
	double * terrain;

	Terrain DiamondSquareIter() const;
public:
	Terrain();
	Terrain(long width, long height);
	Terrain(double * initdata, long width, long height);
	Terrain(const Terrain& t);
	Terrain& operator= (const Terrain &t);
	~Terrain();

	void FlattenWater(long waterLevel);
	Terrain DiamondSquare(int iterations) const;
	VertexBuffer Triangulate() const;

	double Width() const { return width; }
	double Height() const { return height; }

	friend std::ostream& operator<<(std::ostream&, const Terrain&);

	double& operator() (int i, int j);
	const double& operator() (int i, int j) const;
};