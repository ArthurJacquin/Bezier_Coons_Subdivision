#pragma once
#include <vector>
#include "Vertex.h"


using namespace std;

class Mesh 
{
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	vector<Vertex> normals;
	vector<Vertex> uvs;

	uint32_t VBO;
	uint32_t IBO;

public: 
	Mesh();

	void CalculateIndices(int rows, int cols);
	void CalculateNormals();
	void CalculateUvs();

	void updateBuffers();

	vector<Vertex>& getVertices() { return vertices; }
	vector<uint32_t>& getIndices() { return indices; }
	uint32_t getVBO() { return VBO; }
	uint32_t getIBO() { return IBO; }
};