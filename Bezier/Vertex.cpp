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

Vertex::Vertex(double x, double y, float r, float g, float b)
	:x(x), y(y), r(r), g(g), b(b)
{
}

std::ostream& operator<<(std::ostream& os, const Vertex& obj)
{
	os << "(" << obj.x << "," << obj.y << "," << obj.r << "," << obj.g << "," << obj.b << ")" << std::endl;
	return os;
}
