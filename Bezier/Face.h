#pragma once
#include "Vertex.h"

using namespace std;

class Face {

	vector<Vertex> vertices;
	vector<uint32_t> indices;
	vector<pair<Vertex, Vertex>> edges;

	uint32_t VBO;

public:
	Face();
	Face(vector<Vertex> verts);

	void CalculateNormals();
	void updateBuffers();
	void SetColor(Color col);

	vector<Vertex>& getVertices() { return vertices; }
	vector<uint32_t>& getIndices() { return indices; }
	uint32_t getVBO() { return VBO; }
};