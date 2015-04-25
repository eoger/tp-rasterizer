#include <iostream>
#include <cstdlib>
#include <ctime>

#include "evector.h"
#include "matrix.h"

#include "Color.h"
#include "Light.h"
#include "Terrain.h"
#include "FrameBuffer.h"

using namespace std;

#define HEIGHTMAP_HEIGHT 10
#define HEIGHTMAP_WIDTH 13

#define WATER_LEVEL 590
#define SNOW_LEVEL 850
#define ROCK_LEVEL 700

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH  640

#define X_SIZE 1000
#define Y_SIZE 600

#define CAMERA_FOCAL 325.0

#define DIAMOND_ITERATIONS 4

int main(void)
{
	srand(static_cast<int>(time(NULL)));

	double heightdata[] = {
		670, 672, 670, 675, 690, 680, 650, 675, 690, 680, 700, 892, 895,
		680, 665, 640, 630, 650, 645, 630, 628, 648, 650, 680, 875, 893,
		630, 615, 585, 580, 585, 600, 590, 610, 603, 603, 630, 850, 895,
		595, 568, 555, 560, 575, 580, 575, 570, 580, 610, 625, 800, 850,
		550, 540, 538, 550, 595, 575, 600, 570, 575, 620, 613, 700, 730,
		525, 530, 538, 550, 603, 625, 615, 580, 570, 610, 590, 610, 720,
		545, 540, 538, 597, 575, 605, 593, 578, 573, 593, 608, 595, 695,
		615, 560, 543, 579, 569, 560, 563, 570, 580, 595, 619, 638, 650,
		625, 598, 560, 559, 586, 558, 578, 585, 600, 615, 655, 680, 683,
		610, 600, 610, 605, 615, 618, 625, 638, 648, 665, 680, 700, 705
	};

	// ## INITIATISATION TERRAIN ##
	Terrain terrain(heightdata, HEIGHTMAP_WIDTH, HEIGHTMAP_HEIGHT);
	
	// ## DIAMOND SQUARE ##
	terrain = terrain.DiamondSquare(DIAMOND_ITERATIONS);

	// ## APPLATISSEMENT DE L'EAU (oui, oui) ##
	terrain.FlattenWater(WATER_LEVEL);

	// ## TRIANGULATION ##
	VertexBuffer vbuffer = terrain.Triangulate();
	/*
	Test ZBuffer
	VertexBuffer vbuffer;
	
	vbuffer += Point3D(40, 40, 600);
	vbuffer += Point3D(70, 95, 600);
	vbuffer += Point3D(140, 80, 600);
	
	vbuffer += Point3D(40, 40, 600);
	vbuffer += Point3D(70, 95, 600);
	vbuffer += Point3D(30, 140, 600);
	
	vbuffer += Point3D(70, 30, 600);
	vbuffer += Point3D(110, 55, 600);
	vbuffer += Point3D(40, 160, 600);*/

	// ## RESCALE DES AXES X ET Y ##
	double scalex = X_SIZE / terrain.Width();
	double scaley = Y_SIZE / terrain.Height();
	vbuffer.RescaleXY(scalex, scaley);
	
	// ## COLORISATION + LIGHTNING ##
	Vector projOrig(1200, 100, 1200);
	Vector camTarget(X_SIZE / 2.0, Y_SIZE / 2.0 + 200, 600 - 200);
	Light sun(Vector(0, -100, 1000), Color::RGB255(255, 255, 255), 0.15);

	vbuffer.Colorize(WATER_LEVEL, SNOW_LEVEL, ROCK_LEVEL, sun, projOrig);

	// ## PROJECTION PERSPECTIVE ##

	// Etape 1 : Translation O -> PO
	Matrix4 T = Matrix4::Translate(projOrig);

	// Etape 2 : Rotation
	Vector w = projOrig - camTarget;
	Normalize(w);
	Vector k(0, 0, 1.0);
	Vector u = k / w;
	Normalize(u);

	Vector v = w / u;

	Matrix4 R = Matrix4::Identity;
	R(0, 0) = u[0]; R(0, 1) = v[0]; R(0, 2) = w[0];
	R(1, 0) = u[1];	R(1, 1) = v[1];	R(1, 2) = w[1];
	R(2, 0) = u[2];	R(2, 1) = v[2];	R(2, 2) = w[2];

	// Etape 3 : Projection
	Matrix4 P = Matrix4::Identity;
	P(3, 2) = 1.0 / CAMERA_FOCAL;

	// Etape 4 : Offset (centre de projection = coordonnées négatives)
	Vector reframe(SCREEN_HEIGHT/2.0, SCREEN_WIDTH/2.0, 0);
	Matrix4 O = Matrix4::Translate(reframe);

	// Etape 5 : Application des transformations
	Matrix4 Tr = O * P * Inverse(R) * Inverse(T);
	vbuffer.Transform(Tr);

	// ## AFFICHAGE DES TRIANGLES ##

	FrameBuffer fb(SCREEN_WIDTH, SCREEN_HEIGHT, Color::RGB255(255, 255, 255));
	
	if(vbuffer.Render(fb, "testprojection.png"))
		std::cout << "Export image ok" << std::endl;
	else
		std::cout << "Export Image failed" << std::endl;
	
	return 0;
}
