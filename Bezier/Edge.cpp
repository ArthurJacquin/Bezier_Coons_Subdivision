#include "Edge.h"

Edge::Edge()
{
	p0 = nullptr;
	p1 = nullptr;
}

Edge::Edge(Vertex* pt0, Vertex* pt1, Edge* parent)
	:p0(pt0), p1(pt1), parent(parent)
{
}

bool Edge::operator==(const Edge& e)
{
	if (*p0 == *(e).p0 && *p1 == *(e).p1)
		return true;
	if (*p0 == *(e).p1 && *p1 == *(e).p0)
		return true;

	return false;
}

bool Edge::HavetheSameParent(const Edge& e)
{
	return *parent == *e.parent;
}
