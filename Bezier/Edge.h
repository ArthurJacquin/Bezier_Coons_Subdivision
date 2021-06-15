#pragma once
#include "Vertex.h"

struct Edge {

	Vertex* p0;
	Vertex* p1;

	//Catmull clark
	Vertex* edgePoint;

	Edge* parent;

	Edge();
	Edge(Vertex* pt1, Vertex* pt2, Edge* parent = nullptr);

	bool operator==(const Edge& e);

	bool HavetheSameParent(const Edge& e);
};