#include "Subdivision.h"
#include <cmath> 
#include <map>
#include <unordered_set>
#include <algorithm>


#define PI 3.141592

vector<Face> DooSabin(vector<Face> inputFaces)
{
	vector<Face> outputFaces;

	//Generatation des faces correspondantes aux existantes (F-faces)
	for (size_t f = 0; f < inputFaces.size(); f++)
	{
		vector<Vertex*> pts = inputFaces[f].getVertices();
		vector<Vertex*> newPts;
		int n = pts.size();
		float alpha = 0;

		for (size_t i = 0; i < n; i++) //Pour chaque point de la face
		{
			Vec3 newPos;
			for (size_t j = 0; j < n; j++) //Moyenne des points 
			{
				if (i == j)
				{
					alpha = (n + 5.0f) / (4.0f * n);
				}
				else
				{
					alpha = (3.0f + 2.0f * cos(2.0f * PI * ((i * 3 + j) % 4) / n)) / (4.0f * n);
				}

				newPos += pts[j]->GetPos() * alpha;
			}

			newPts.push_back(new Vertex(newPos, Color(1, 0, 0), pts[i]));
		}

		outputFaces.push_back(Face(newPts, Color(1, 0, 0), &inputFaces[f]));
	}

	//Generation des faces correspondants aux edges(E-faces)
	vector<Face> newFaces;
	int Nbface = outputFaces.size();
	for (size_t i = 0; i < Nbface; i++)
	{
		Face* f0 = &outputFaces[i];
		for (size_t j = 0; j < outputFaces[i].getEdges().size(); j++)//Pour chaque edge
		{
			Edge* e0 = f0->getEdges()[j];
			if (e0->parent == nullptr)
				continue;

			//On cherche l'edge qui à le meme parent (pas besoin de repasser par les faces deja traitées)
			for (size_t k = i; k < Nbface; k++)
			{
				Face* f1 = &outputFaces[k];
				if (f0 == f1)
					continue;

				bool found = false;
				for (size_t l = 0; l < outputFaces[k].getEdges().size(); l++)
				{
					Edge* e1 = f1->getEdges()[l];
					if (e0 == e1 || e1->parent == nullptr)
						continue;

					if (e0->HavetheSameParent(*e1)) //Si elles sont issues du meme edge
					{
						//On vérifie le sens des edges
						if((e0->p0->GetPos() - e0->p1->GetPos()).dot(e1->p0->GetPos() - e1->p1->GetPos()) < 0)
							newFaces.push_back(Face({ e0->p0, e0->p1, e1->p0, e1->p1 }, Color(0, 1, 0)));
						else
							newFaces.push_back(Face({ e0->p0, e0->p1, e1->p1, e1->p0 }, Color(0, 1, 0)));

						e0->parent = nullptr;
						e1->parent = nullptr;
						found = true;
						break;
					}
				}

				if (found)
					break;
			}
		}
	}

	//Generation des faces correspondantes aux vertices (V-Faces)
	for (size_t i = 0; i < Nbface; i++)
	{
		Face* f0 = &outputFaces[i];
		for (size_t j = 0; j < outputFaces[i].getVertices().size(); j++)//Pour chaque edge
		{
			Vertex* v0 = f0->getVertices()[j];
			if (v0->parent == nullptr)
				continue;

			vector<Vertex*> ptsFound;
			ptsFound.push_back(v0);
			//On cherche le vertex qui à le meme parent (pas besoin de repasser par les faces deja traitées)
			for (size_t k = i; k < Nbface; k++)
			{
				Face* f1 = &outputFaces[k];
				if (f0 == f1)
					continue;

				for (size_t l = 0; l < outputFaces[k].getVertices().size(); l++)
				{
					Vertex* v1 = f1->getVertices()[l];
					if (v0 == v1 || v1->parent == nullptr)
						continue;

					if (v0->HaveTheSameParent(*v1)) //Si elles sont issues du meme vertex
					{
						ptsFound.push_back(v1);
						break;
					}
				}
			}

			//On enleve le parent des points traités
			for (size_t k = 0; k < ptsFound.size(); k++)
			{
				ptsFound[k]->parent = nullptr;
			}

			newFaces.push_back(Face({ (ptsFound[0]), (ptsFound[1]), (ptsFound[2]) }, Color(0, 0, 1)));
		}
	}

	//Ajout des nouvelles faces
	for (size_t i = 0; i < newFaces.size(); i++)
	{
		outputFaces.push_back(newFaces[i]);
	}

	return outputFaces;
}

vector<Face> CatmullClark(vector<Face> inputFaces)
{
	vector<Face>catFaces;

	//compute average face point
	for (size_t i = 0; i < inputFaces.size(); i++)
	{
		Vertex averageFacePoint;

		for (size_t j = 0; j < inputFaces[i].getVertices().size(); j++)
		{
			averageFacePoint = averageFacePoint + *(inputFaces[i].getVertices()[j]);
		}
		
		inputFaces[i].setFacePoint(new Vertex(averageFacePoint / (float)inputFaces[i].getVertices().size()));
	}

	//compute edge point
	for (size_t i = 0; i < inputFaces.size(); i++)
	{
		vector<Face*> nFaces;
		for (size_t j = 0; j < inputFaces[i].getEdges().size(); j++)
		{
			Vertex averageEdgePoint;
			nFaces = getNeighborFaces(inputFaces, inputFaces[i].getEdges()[j]);
			for (size_t f = 0; f < nFaces.size(); f++)
			{
				averageEdgePoint = averageEdgePoint + *nFaces[f]->getFacePoint();
			}

			Vertex p0 = *inputFaces[i].getEdges()[j]->p0;
			Vertex p1 = *inputFaces[i].getEdges()[j]->p1;
			averageEdgePoint = (averageEdgePoint + p0 + p1) / (float)(nFaces.size() + 2.f);
			inputFaces[i].getEdges()[j]->edgePoint = new Vertex(averageEdgePoint);
		}
	}

	//compute vertex point
	for (size_t i = 0; i < inputFaces.size(); i++)
	{
		for (size_t v = 0; v < inputFaces[i].getVertices().size(); v++)
		{
			vector<Face> faces;
			vector<Edge*> edges;
			getNeighborVertex(faces, edges, inputFaces, inputFaces[i].getVertices()[v]);
			deleteDuplicateInVector(edges);
			Vertex Q;
			Vertex R;
			float n = (float)edges.size();

			for (size_t f = 0; f < faces.size(); f++)
			{
				Q = Q + *faces[f].getFacePoint();
			}

			Q = Q / faces.size();

			//average all of mid points
			for (size_t r = 0; r < edges.size(); r++)
			{
				R = R + (*edges[r]->p0 + *edges[r]->p1) / 2.f;
			}

			R = R / edges.size();

			Vertex inputPoint = *inputFaces[i].getVertices()[v];
			Vertex* newPt = new Vertex((Q * (1.f / n)) + (R * (2.f / n)) + (inputPoint * (n - 3.f) / n));
			inputFaces[i].getVertices()[v]->vPrime = newPt;
		}
	}

	//connect new faces
	for (size_t i = 0; i < inputFaces.size(); i++)
	{
		for (size_t e = 0; e < inputFaces[i].getEdges().size(); e++)
		{
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

			Face newFace({ inputFaces[i].getFacePoint(), inputFaces[i].getEdges()[e]->edgePoint,
							inputFaces[i].getEdges()[e]->p1->vPrime, inputFaces[i].getEdges()[(e + 1) % inputFaces[i].getEdges().size()]->edgePoint },
							Color(r, g, b));
			catFaces.push_back(newFace);
		}
	}

	return catFaces;
}

vector<Face*> getNeighborFaces(vector<Face>& face, const Edge* const edge)
{
	vector<Face*> neighborFaces;

	for (size_t f = 0; f < face.size(); f++)
	{
		for (size_t e = 0; e < face[f].getEdges().size(); e++)
		{
			if (*face[f].getEdges()[e] == *edge)
			{
				neighborFaces.push_back(&face[f]);
				break;
			}
		}
	}

	return neighborFaces;
}

void getNeighborVertex(vector<Face>& faces, vector<Edge*>& edges, const vector<Face>& inputFaces, const Vertex* const v)
{
	for (size_t f = 0; f < inputFaces.size(); f++)
	{
		bool inFaces = false;

		for (size_t e = 0; e < inputFaces[f].getEdges().size(); e++)
		{
			if (*v == *inputFaces[f].getEdges()[e]->p0 || *v == *inputFaces[f].getEdges()[e]->p1)
			{
				edges.push_back(inputFaces[f].getEdges()[e]);
				inFaces = true;
			}
		}

		if (inFaces == true)
		{
			faces.push_back(inputFaces[f]);
		}
	}
}

vector<Vertex*> getNeighborVertices(const vector<Face>& inputFaces, const Vertex* const v)
{
	vector<Vertex*> output;

	for (size_t i = 0; i < inputFaces.size(); i++)
	{
		for (size_t j = 0; j < inputFaces[i].getEdges().size(); j++)
		{
			if (*inputFaces[i].getEdges()[j]->p0 == *v)
				output.push_back(inputFaces[i].getEdges()[j]->p1);
			if (*inputFaces[i].getEdges()[j]->p1 == *v)
				output.push_back(inputFaces[i].getEdges()[j]->p0);
		}
	}

	deleteDuplicateInVector(output);
	return output;
}

vector<Face> Kobelt(vector<Face> inputFaces)
{
	vector<Face> outputFaces;

	//Calcul des centres
	for (size_t i = 0; i < inputFaces.size(); i++)
	{
		Face* face = &inputFaces[i];
		int N = face->getVertices().size();
		
		Vertex center;
		for (size_t j = 0; j < N; j++)
		{
			center += *face->getVertices()[j];
		}
		center /= N;
		face->setFacePoint(new Vertex(center));

		//Connection aux vertex du triangle
		for (size_t j = 0; j < N; j++)
		{
			outputFaces.push_back(Face({ face->getVertices()[j], face->getVertices()[(j + 1) % N], face->getFacePoint() }, Color(1, 0, 0)));
		}
	}

	//Flipping
	map<Face*, Face> newFaces;
	for (size_t i = 0; i < inputFaces.size(); i++)
	{
		Face face = inputFaces[i];
		int N = face.getEdges().size();

		vector<Face*> neighBorFacesInput;
		vector<Face*> neighBorFacesOutput;
		for (size_t j = 0; j < N; j++)
		{
			neighBorFacesInput = getNeighborFaces(inputFaces, face.getEdges()[j]);

			Face f0 = Face({ neighBorFacesInput[0]->getFacePoint(), neighBorFacesInput[1]->getFacePoint(), face.getEdges()[j]->p0 }, Color(1, 0, 0));
			Face f1 = Face({ neighBorFacesInput[0]->getFacePoint(), neighBorFacesInput[1]->getFacePoint(), face.getEdges()[j]->p1 }, Color(0, 1, 0));

			neighBorFacesOutput = getNeighborFaces(outputFaces, face.getEdges()[j]);

			if(newFaces.find(neighBorFacesOutput[0]) == newFaces.end())
				newFaces.insert(make_pair(neighBorFacesOutput[0], f0));
			if (newFaces.find(neighBorFacesOutput[1]) == newFaces.end())
				newFaces.insert(make_pair(neighBorFacesOutput[1], f1));
		}
	}

	//Creation des nouvelles faces
	for (size_t i = 0; i < outputFaces.size(); i++)
	{
		Face f = newFaces[&outputFaces[i]];
		outputFaces[i] = f;
	}

	//Perturbation
	vector<Vertex*> newVertices;
	for (size_t i = 0; i < inputFaces.size(); i++)
	{
		Face face = inputFaces[i];
		int N = face.getVertices().size();

		vector<Vertex*> neighbors;
		for (size_t j = 0; j < N; j++)
		{
			neighbors = getNeighborVertices(inputFaces, face.getVertices()[j]);
			int n = neighbors.size();
			float alpha = (4 - 2 * cos(2 * PI / n)) / 9.0f;

			Vertex sumV;
			for (size_t k = 0; k < n; k++)
			{
				sumV += *neighbors[k];
			}

			Vertex V = *face.getVertices()[j];
			Vertex vPrime = V * (1 - alpha) + sumV * alpha / n;
			vPrime.parent = face.getVertices()[j];
			vPrime.setColor(Color(0, 0, 1));
			newVertices.push_back(new Vertex(vPrime));
		}
	}

	//Modification des points pertubés dans l'output
	for (size_t i = 0; i < outputFaces.size(); i++)
	{
		Face* face = &outputFaces[i];
		int N = face->getVertices().size();

		for (size_t j = 0; j < N; j++)
		{
			for (size_t k = 0; k < newVertices.size(); k++)
			{
				if (*newVertices[k]->parent == *face->getVertices()[j]) {
					face->getVertices()[j] = newVertices[k];
					break;
				}
			}
		}

		outputFaces[i] = Face({ face->getVertices()[0], face->getVertices()[1], face->getVertices()[2] });
	}

	//Des couleurs
	for (size_t i = 0; i < outputFaces.size(); i++)
	{
		outputFaces[i].SetColor(Color(), true);
	}
	
	return outputFaces;
}

vector<Face> LoopAlgo(vector<Face> inputFaces)
{
	vector<Face> outputFaces;

	//compute even vertices
	//for each face in cube
	for (size_t f = 0; f < inputFaces.size(); f++)
	{
		//compute even vertices
		//for each vertices in face
		for (size_t v = 0; v < inputFaces[f].getVertices().size(); v++)
		{
			Vertex* vCurrent = inputFaces[f].getVertices()[v];
			vector<Face> faces;
			vector<Edge*> edges;
			vector<Vertex> adjacentVertices;
			//get adjacent edges at vertex
			getNeighborVertex(faces, edges, inputFaces, vCurrent);
			deleteDuplicateInVector(edges);

			//get all other vertices
			for (size_t i = 0; i < edges.size(); i++)
			{
				if (*inputFaces[f].getVertices()[v] == *edges[i]->p0)
				{
					adjacentVertices.push_back(*edges[i]->p1);
				}
				else if (*inputFaces[f].getVertices()[v] == *edges[i]->p1)
				{
					adjacentVertices.push_back(*edges[i]->p0);
				}
			}

			//compute alpha
			float alpha = 0;
			if (adjacentVertices.size() == 3)
				alpha = 3.f / 16.f;
			else
				alpha = 1.f / adjacentVertices.size() * (5.f / 8.f - pow(3.f / 8.f + 1.f / 4.f * cos(2.f * PI / adjacentVertices.size()), 2.f));

			//get sum of vertices
			Vertex sum;
			for (size_t i = 0; i < adjacentVertices.size(); i++)
			{
				sum = sum + adjacentVertices[i];
			}

			//compute vPrime
			Vertex vPrime = (*vCurrent) * (1.f - adjacentVertices.size() * alpha) + sum * alpha;
			inputFaces[f].getEvenVertices().push_back(new Vertex(vPrime));
		}

		//compute odd vertices 
		//for edges vertices in face
		for (size_t j = 0; j < inputFaces[f].getEdges().size(); j++)
		{
			vector<Face*> neighborFaces;
			//get all faces near edge
			neighborFaces = getNeighborFaces(inputFaces, inputFaces[f].getEdges()[j]);

			//vLeft and vRight
			vector<Vertex*> vNotInEdge = VertexNotInEdge(neighborFaces, inputFaces[f].getEdges()[j]);
			Vertex e;
			if(vNotInEdge.size() > 1)
				e = (*inputFaces[f].getEdges()[j]->p0 + *inputFaces[f].getEdges()[j]->p1) * 3 / 8
						 + (*vNotInEdge[0] + *vNotInEdge[1]) * 1 / 8;
			else
				e = (*inputFaces[f].getEdges()[j]->p0 + *inputFaces[f].getEdges()[j]->p1) * 3 / 8
				+ (*vNotInEdge[0]) * 1 / 8;

			inputFaces[f].getOddVertices().push_back(new Vertex(e));
		}
	}
	 
	//create subdivide faces
	for (size_t f = 0; f < inputFaces.size() ; f++)
	{
		outputFaces.push_back(createFace(inputFaces[f].getEvenVertices()[0], 
			                             inputFaces[f].getOddVertices()[0], 
			                             inputFaces[f].getOddVertices()[2]));

		outputFaces.push_back(createFace(inputFaces[f].getOddVertices()[0],
			                             inputFaces[f].getEvenVertices()[1],
			                             inputFaces[f].getOddVertices()[1]));

		outputFaces.push_back(createFace(inputFaces[f].getOddVertices()[0], //e1
			                             inputFaces[f].getOddVertices()[1], //e2
			                             inputFaces[f].getOddVertices()[2]));//e3

		outputFaces.push_back(createFace(inputFaces[f].getOddVertices()[1], //e2
										 inputFaces[f].getEvenVertices()[2], //v1
										 inputFaces[f].getOddVertices()[2]));//e3
	}

	return outputFaces;
}

vector<Vertex*> VertexNotInEdge(const vector<Face*>& faces, const Edge* e)
{
	vector<Vertex*> pts;
	for (size_t i = 0; i < faces.size(); i++)
	{
		for (size_t j = 0; j < faces[i]->getVertices().size(); j++)
		{
			Vertex* pt = faces[i]->getVertices()[j];
			if (*pt != *e->p0
				&& *pt != *e->p1)
			{
				pts.push_back(faces[i]->getVertices()[j]);
			}
		}
	}

	return pts;
}

Face createFace(Vertex* v1, Vertex* v2, Vertex* v3)
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	Face newFace1({ v1,
				    v2,  
				    v3 },
		Color(r, g, b));
	return newFace1;
}

template<typename T>
void deleteDuplicateInVector(vector<T*>& vec)
{   
	for (size_t i = 0; i < vec.size(); i++)
	{
		for (size_t j = i + 1; j < vec.size(); j++)
		{
			if (*vec[i] == *vec[j])
			{
				vec.erase(vec.begin() + j);
				j -= 1;
			}
		}
	}
	
}