#pragma once
#include "Edge.h"

using namespace std;

class Face {

	vector<Vertex*> vertices;
	vector<uint32_t> indices;
	vector<Edge*> edges;

	//Catmull Clark
	Vertex* facePoint;

	Face* parent;

	uint32_t VBO;
	vector<Vertex> bufferPts;

public:
	Face();
	Face(vector<Vertex*> verts, Color col = Color(1, 1, 1), Face* p = nullptr);

	void CalculateNormals();
	void updateBuffers();
	void SetColor(Color col);

	vector<Vertex*>& getVertices() { return vertices; }
	vector<uint32_t>& getIndices() { return indices; }
	vector<Edge*>& getEdges() { return edges; }
	uint32_t getVBO() { return VBO; }

	Vertex* getFacePoint() { return facePoint; }
	void setFacePoint(Vertex* v) { facePoint = v; }

	void updateBufferPoints();
};