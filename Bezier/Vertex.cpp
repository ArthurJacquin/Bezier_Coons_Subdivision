#include "Vertex.h"

extern int width, height;

Vertex::Vertex()
{
	x = 0;
	y = 0;
	z = 0;
	r = 0;
	g = 0;
	b = 0;

	normal = Vec3(0.0f, 0.0f, 1.0f);
	neighborVertices.reserve(0);
}

Vertex::Vertex(Color col)
{
	r = col.x;
	g = col.y;
	b = col.z;

	normal = Vec3(0.0f, 0.0f, 1.0f);
	neighborVertices.reserve(0);
}

Vertex::Vertex(double x, double y, double z, float r, float g, float b)
	:x(x), y(y), z(z), r(r), g(g), b(b)
{
	normal = Vec3(0.0f, 0.0f, 1.0f);
	neighborVertices.reserve(0);
}

Vertex::Vertex(double x, double y, double z)
	: x(x), y(y), z(z)
{
	r = 1;
	g = 1;
	b = 1;

	normal = Vec3(0.0f, 0.0f, 1.0f);
	neighborVertices.reserve(0);
}

void Vertex::setColor(Color color)
{
	this->r = color.x;
	this->g = color.y;
	this->b = color.z;
}

void Vertex::setPositionUsingMouse(double x, double y)
{
	this->x = -1.0f + 2 * x / width;
	this->y = 1.0f - 2 * y / height;
	this->z = 0.0;
}

void Vertex::setNormal(Vec3 n)
{
	normal = n;
}

std::ostream& operator<<(std::ostream& os, const Vertex& obj)
{
	os << "(" << obj.x << "," << obj.y << "," << obj.z << "," << obj.r << "," << obj.g << "," << obj.b << ")" << std::endl;
	return os;
}
