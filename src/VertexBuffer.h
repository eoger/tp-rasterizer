#pragma once

#include <vector>
#include "FrameBuffer.h"
#include "Light.h"
#include "matrix.h"

class VertexBuffer
{
protected:
	vector<Point3D> vertices;
public:
	void RescaleXY(double scalex, double scaley);
	void Transform(Matrix4 T);
	void Colorize(int waterLevel, int snowLevel, int rockLevel, Light light, Vector viewDir);
	bool Render(FrameBuffer& fbuf, string filename);
	VertexBuffer& operator+= (const Point3D&);


	friend std::ostream& operator<<(std::ostream&, const VertexBuffer&);

	VertexBuffer();
	virtual ~VertexBuffer();
};

