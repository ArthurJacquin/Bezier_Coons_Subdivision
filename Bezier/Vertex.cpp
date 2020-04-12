#include "Vertex.h"

extern int width, height;

Vertex::Vertex()
{
	x = 0;
	y = 0;
	r = 0;
	g = 0;
	b = 0;
}

Vertex::Vertex(Color col)
{
	r = col.x;
	g = col.y;
	b = col.z;
}

Vertex::Vertex(double x, double y, float r, float g, float b)
	:x(x), y(y), r(r), g(g), b(b)
{
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
}

std::ostream& operator<<(std::ostream& os, const Vertex& obj)
{
	os << "(" << obj.x << "," << obj.y << "," << obj.r << "," << obj.g << "," << obj.b << ")" << std::endl;
	return os;
}
