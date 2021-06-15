#include "Subdivision.h"

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

			//On cherche l'edge qui � le meme parent (pas besoin de repasser par les faces deja trait�es)
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
						//On v�rifie le sens des edges
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
			//On cherche le vertex qui � le meme parent (pas besoin de repasser par les faces deja trait�es)
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

			//On enleve le parent des points trait�s
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
		
		inputFaces[i].setFacePoint(new Vertex(averageFacePoint / inputFaces[i].getVertices().size()));
	}

	//compute edge point
	for (size_t i = 0; i < inputFaces.size(); i++)
	{
		Vertex averageEdgePoint;
		vector<Face> nFaces;
		for (size_t j = 0; j < inputFaces[i].getEdges().size(); j++)
		{
			nFaces = getNeighborFaces(inputFaces, inputFaces[i].getEdges()[j]);
			for (size_t f = 0; f < nFaces.size(); f++)
			{
				averageEdgePoint = averageEdgePoint + *nFaces[f].getFacePoint();
			}

			averageEdgePoint = (averageEdgePoint + *inputFaces[i].getEdges()[j]->p0 + *inputFaces[i].getEdges()[j]->p1) / (nFaces.size() + 2);
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
			getNeighborVertex(faces, edges, inputFaces[i].getVertices()[v]);
			Vertex Q;
			Vertex R;
			int n = edges.size();

			for (size_t f = 0; f < faces.size(); f++)
			{
				Q = Q + *faces[f].getFacePoint();
			}

			Q = Q / faces.size();

			for (size_t r = 0; r < edges.size(); r++)
			{
				R = R + *edges[r]->edgePoint;
			}

			R = R / edges.size();

			inputFaces[i].getVertices()[v]->vPrime = new Vertex( (Q * 1 / n) + (R * 2 / n) + (*inputFaces[i].getVertices()[v] * (n - 3) / n) );
		}
	}

	//connect new faces
	for (size_t i = 0; i < inputFaces.size(); i++)
	{
		for (size_t e = 0; e < inputFaces[i].getEdges().size(); e++)
		{
			Face newFace({ inputFaces[i].getFacePoint(), inputFaces[i].getEdges()[(e + 1) % inputFaces[i].getEdges().size()]->edgePoint,
							inputFaces[i].getEdges()[e]->p1->vPrime, inputFaces[i].getEdges()[e]->edgePoint });
			catFaces.push_back(newFace);
		}
	}

	return catFaces;
}

vector<Face> getNeighborFaces(vector<Face> face, Edge* edge)
{
	vector<Face> neighborFaces;

	for (size_t f = 0; f < face.size(); f++)
	{
		for (size_t e = 0; e < face[f].getEdges().size(); e++)
		{
			if (*face[f].getEdges()[e] == *edge)
			{
				neighborFaces.push_back(face[f]);
				break;
			}
		}
	}

	return neighborFaces;
}

void getNeighborVertex(vector<Face>& faces, vector<Edge*>& edges, Vertex* v)
{
	for (size_t f = 0; f < faces.size(); f++)
	{
		bool inFaces = false;

		for (size_t e = 0; e < faces[f].getEdges().size(); e++)
		{
			if (v == faces[f].getEdges()[e]->p0 || v == faces[f].getEdges()[e]->p1)
			{
				edges.push_back(faces[f].getEdges()[e]);
				inFaces = true;
			}
		}

		if (inFaces == true)
		{
			faces.push_back(faces[f]);
		}

	}

}