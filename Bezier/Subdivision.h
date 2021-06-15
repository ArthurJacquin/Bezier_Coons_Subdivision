#pragma once
#include "Face.h"

using namespace std;

vector<Face> DooSabin(vector<Face> inputFaces);

vector<Face> CatmullClark(vector<Face> inputFaces);
vector<Face*> getNeighborFaces(vector<Face> face, const Edge* const edge);
void getNeighborVertex(vector<Face>& faces, vector<Edge*>& edges, const vector<Face>& inputFaces, const Vertex* const v);

vector<Face> Kobelt(vector<Face> inputFaces);

vector<Face> LoopAlgo(vector<Face> inputFaces);

vector<Vertex*> VertexNotInEdge(const vector<Face*>& faces, const Edge* e);