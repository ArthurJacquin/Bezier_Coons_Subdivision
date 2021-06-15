#pragma once
#include "Face.h"

using namespace std;

vector<Face> DooSabin(vector<Face> inputFaces);

vector<Face> CatmullClark(vector<Face> inputFaces);
vector<Face> getNeighborFaces(vector<Face> face, Edge* edge);
void getNeighborVertex(vector<Face>& faces, vector<Edge*>& edges, Vertex* v);