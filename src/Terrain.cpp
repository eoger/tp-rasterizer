#include "Terrain.h"

#include <algorithm>
#include <cassert>

#define DIAMOND_RAND_MIN -2
#define DIAMOND_RAND_MAX 2

Terrain::Terrain() : width(0), height(0)
{
}
Terrain::Terrain(long w, long h) : width(w), height(h)
{
	terrain = new double[width*height]();
}

Terrain::Terrain(double * initdata, long w, long h) : width(w), height(h)
{
	terrain = new double[width*height];
	std::copy(initdata, initdata+(width*height), terrain);
}

Terrain::Terrain(const Terrain& t) : width(t.width), height(t.height)
{
	terrain = new double[width*height];
	std::copy(t.terrain, t.terrain + (width*height), terrain);
}

Terrain &Terrain::operator= (const Terrain &t)
{
	width = t.width;
	height = t.height;

	delete[] terrain;
	terrain = new double[width*height];
	std::copy(t.terrain, t.terrain + (width*height), terrain);

	return *this;
}


Terrain::~Terrain()
{
	delete[] terrain;
}

void Terrain::FlattenWater(long waterLevel)
{
	for (long i = 0; i < width*height; i++)
	{
		if (terrain[i] < waterLevel)
		{
			terrain[i] = waterLevel;
		}
	}
}

Terrain Terrain::DiamondSquareIter() const
{
	// Calcul nouvelle taille terrain
	long newWidth = 2 * width - 1;
	long newHeight = 2 * height - 1;


	Terrain t(newWidth, newHeight);

	// Initialisation des anciennes valeurs
	for (long j = 1; j <= height; j++)
	{
		for (long i = 1; i <= width; i++)
		{
			t(2 * j - 2, 2 * i - 2) = (*this)(j - 1, i - 1);
		}
	}

	// Passe "diamond"
	for (long j = 1; j < newHeight; j += 2)
	{
		for (long i = 1; i < newWidth; i += 2)
		{
			double val = (t(j - 1, i - 1) + t(j - 1, i + 1) + t(j + 1, i - 1) + t(j + 1, i + 1)) / 4.0;
			val += rand() / (RAND_MAX / (abs(DIAMOND_RAND_MIN) + DIAMOND_RAND_MAX)) + DIAMOND_RAND_MIN;
			t(j, i) = val;
		}
	}

	int start = 0;
	// Passe "square"
	for (long j = 0; j < newHeight; j++)
	{
		start = 1 - start;
		for (long i = start; i < newWidth; i += 2)
		{
			double val;
			if (i == 0) {
				val = (t(j, i + 1) + t(j - 1, i) + t(j + 1, i)) / 3.0;
			}
			else if (i == (newWidth - 1)) {
				val = (t(j, i - 1) + t(j - 1, i) + t(j + 1, i)) / 3.0;
			}
			else if (j == 0) {
				val = (t(j, i - 1) + t(j, i + 1) + t(j + 1, i)) / 3.0;
			}
			else if (j == (newHeight - 1)) {
				val = (t(j, i - 1) + t(j, i + 1) + t(j - 1, i)) / 3.0;
			}
			else {
				val = (t(j, i - 1) + t(j, i + 1) + t(j - 1, i) + t(j + 1, i)) / 4.0;
			}

			val += rand() / (RAND_MAX / (abs(DIAMOND_RAND_MIN) + DIAMOND_RAND_MAX)) + DIAMOND_RAND_MIN;
			t(j, i) = val;
		}
	}

	return t;
}

Terrain Terrain::DiamondSquare(int iterations) const
{
	Terrain t(*this);
	for (int i = 0; i < iterations; i++)
	{
		t = t.DiamondSquareIter();
	}
	return t;
}

VertexBuffer Terrain::Triangulate() const
{
	VertexBuffer vbuf;

	for (long j = 0; j<height - 1; j++)
	{
		for (long i = 0; i<width - 1; i++) {

			vbuf += Point3D(j, i, (*this)(j, i));
			vbuf += Point3D(j, i + 1, (*this)(j, i + 1));
			vbuf += Point3D(j + 1, i, (*this)(j + 1, i));

			vbuf += Point3D(j, i + 1, (*this)(j, i + 1));
			vbuf += Point3D(j + 1, i + 1, (*this)(j + 1, i + 1));
			vbuf += Point3D(j + 1, i, (*this)(j + 1, i));
		}
	}

	return vbuf;
}

std::ostream& operator<<(std::ostream& s, const Terrain& t)
{
	for (long i = 0; i < t.Height(); i++)
	{
		for (long j = 0; j < t.Width(); j++)
		{
			s << t(i, j) << " ";
		}
		s << std::endl;
	}
	return s;
}

double& Terrain::operator() (int i, int j) {
	assert(i*width + j < height * width);
	return terrain[i*width + j];
}

const double& Terrain::operator() (int i, int j) const {
	assert(i*width + j < height * width);
	return terrain[i*width + j];
}