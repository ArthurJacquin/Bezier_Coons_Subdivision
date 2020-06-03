#pragma once
#include <vector>
#include "Vertex.h"
#include "Vec3.h"

using namespace std;

struct Mesh 
{
	vector<Vertex> vertices;
	vector<uint32_t> indices;

	uint32_t VBO;

	Mesh();

	void CalculateIndices(int rows, int cols);
	void CalculateNormals();

	void updateBuffers();

	vector<Vertex>& getVertices() { return vertices; }
	vector<uint32_t>& getIndices() { return indices; }
	uint32_t getVBO() { return VBO; }
};