#pragma once
#include <iostream>
#include "Color.h"
#include "Vec3.h"

struct Vertex
{
	double x, y, z;
	float r, g, b;
	Vec3 normal;

	Vertex();
	Vertex(Color col);
	Vertex(double x, double y, double z, float r, float g, float b);
	Vertex(double x, double y, double z);

	friend std::ostream& operator <<(std::ostream&, const Vertex& obj);
	bool operator!=(const Vertex v) { return x != v.x || y != v.y || z != v.z || r != v.r || g != v.g || b != v.b; }
	bool operator==(const Vertex v) { return !(*this != v); }
	Vertex& operator=(Vertex v) { x = v.x; y = v.y; z = v.z; r = v.r; g = v.g; b = v.b; return *this; }

	void setColor(Color color);
	void setPositionUsingMouse(double x, double y);
	void setNormal(Vec3 n);
};