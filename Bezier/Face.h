#pragma once
#include "Edge.h"

using namespace std;

class Face {

	vector<Vertex*> vertices;
	vector<uint32_t> indices;
	vector<Edge*> edges;

	//Catmull Clark
	Vertex* facePoint;

	//Loop Algo
	vector<Vertex*> oddVertices;
	vector<Vertex*> evenVertices;

	Face* parent;

	uint32_t VBO;
	vector<Vertex> bufferPts;

public:
	Face();
	Face(vector<Vertex*> verts, Color col = Color(1, 1, 1), Face* p = nullptr);

	void CalculateNormals();
	void updateBuffers();
	void SetColor(Color col);

	const vector<Vertex*>& getVertices() const { return vertices; }
	const vector<uint32_t>& getIndices() const { return indices; }
	const vector<Edge*>& getEdges() const { return edges; }
	uint32_t getVBO() { return VBO; }

	Vertex* getFacePoint() { return facePoint; }
	vector<Vertex*>& getEvenVertices() { return evenVertices; }
	vector<Vertex*>& getOddVertices() { return oddVertices; }
	void setFacePoint(Vertex* v) { facePoint = v; }

	void updateBufferPoints();
};