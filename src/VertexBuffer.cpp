#include "VertexBuffer.h"
#include <cassert>

#include "Color.h"
#include "FrameBuffer.h"
#include "Material.h"

#define MONTAIN_SLOPE 1.0

VertexBuffer::VertexBuffer()
{
}


VertexBuffer::~VertexBuffer()
{
}


VertexBuffer& VertexBuffer::operator += (const Point3D& v)
{
	vertices.push_back(v);
	return (*this);
}

bool VertexBuffer::Render(FrameBuffer& fb, string filename)
{
	assert(vertices.size() % 3 == 0);

	long nbtris = vertices.size() / 3;

	for (long i = 0; i < nbtris; i++)
	{
		fb.DrawTriangle(
			vertices[i * 3],
			vertices[i * 3 + 1],
			vertices[i * 3 + 2]
			);
	}
	
	return fb.SavePng("testprojection.png");
}

std::ostream& operator<<(std::ostream& s, const VertexBuffer& t)
{
	long nbtris = t.vertices.size() / 3;

	for (long i = 0; i < nbtris; i++)
	{
		s << t.vertices[i * 3] << '\t';
		s << t.vertices[i * 3 + 1] << '\t';
		s << t.vertices[i * 3 + 2] << std::endl;
	}
	return s;
}


void VertexBuffer::RescaleXY(double scalex, double scaley)
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i][0] *= scalex;
		vertices[i][1] *= scaley;
	}
}

void VertexBuffer::Transform(Matrix4 T)
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		Vector v = T * vertices[i];
		vertices[i] = v;
	}
}

void VertexBuffer::Colorize(int waterLevel, int snowLevel, int rockLevel, Light light, Vector camPos)
{
	long nbtris = vertices.size() / 3;

	for (long i = 0; i < nbtris; i++)
	{
		int lowestIdx = i * 3;
		int highestIdx = i * 3;
		for (int j = 1; j < 3; j++)
		{
			if (vertices[i * 3 + j][2] < vertices[lowestIdx][2]) {
				lowestIdx = i * 3 + j;
			}
			else if (vertices[i * 3 + j][2] > vertices[highestIdx][2]) {
				highestIdx = i * 3 + j;
			}
		}

		double slope = (vertices[highestIdx][2] - vertices[lowestIdx][2]) / sqrt(pow((vertices[highestIdx][0] - vertices[lowestIdx][0]), 2) + pow((vertices[highestIdx][1] - vertices[lowestIdx][1]), 2));

		Material mat;

		if (vertices[highestIdx][2] <= waterLevel) {
			mat = Material::Water;
		}
		else if (vertices[highestIdx][2] > snowLevel)
		{
			mat = Material::Snow;
		}
		else if (slope >= MONTAIN_SLOPE || vertices[highestIdx][2] > rockLevel)
		{
			mat = Material::Montain;
		}
		else {
			mat = Material::Grass;
		}
		
		// ## ILLUMINATION PHONG ##

		Vector centroid = (vertices[i * 3] + vertices[i * 3 + 1] + vertices[i * 3 + 2]) / 3;
		Vector normal = Normalized((vertices[i * 3 + 2] - vertices[i * 3]) / (vertices[i * 3 + 1] - vertices[i * 3]));
		Vector surfaceToLight = Normalized(light.pos - centroid);
		Vector surfaceToCamera = Normalized(camPos - centroid);

		// Calcul ambient light
		Color ambient = light.ambientCoef * mat.diffuseColor * light.color;

		// Calcul diffuse
		double diffuseCoef = max(0.0, normal * surfaceToLight);
		Color diffuse = diffuseCoef * mat.diffuseColor * mat.diffusePower * light.color;

		// Calcul speculaire
		double specularCoef = 0.0;
		if (diffuseCoef > 0.0) {
			Vector R = 2.0 * normal * (normal * surfaceToLight) - surfaceToLight;
			specularCoef = pow(max(0.0, surfaceToCamera * R), mat.specularHardness);
		}
		Color specular = specularCoef * mat.specularColor * mat.specularPower * light.color;

		Color col = ambient + diffuse + specular;

		// Tone mapping
		col = Clamp(col, Vector(0, 0, 0), Vector(1, 1, 1));

		vertices[i * 3].color = col;
		vertices[i * 3 + 1].color = col;
		vertices[i * 3 + 2].color = col;
	}
}