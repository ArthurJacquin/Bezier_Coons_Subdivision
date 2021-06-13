#include "Subdivision.h"

#define PI 3.141592

vector<Face> DooSabin(vector<Face> inputFaces)
{
	vector<Face> outputFaces;

	//Generatation des faces correspondantes aux existantes (F-faces)
	for (size_t f = 0; f < inputFaces.size(); f++)
	{
		vector<Vertex> pts = inputFaces[f].getVertices();
		vector<Vertex> newPts;
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

				newPos += pts[j].GetPos() * alpha;
			}

			newPts.push_back(Vertex(newPos, Color(1, 0, 0), &pts[i]));
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
			Edge* e0 = &f0->getEdges()[j];
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
					Edge* e1 = &f1->getEdges()[l];
					if (e0 == e1 || e1->parent == nullptr)
						continue;

					if (e0->HavetheSameParent(*e1)) //Si elles sont issues du meme edge
					{
						//On vérifie le sens des edges
						if((e0->p0.GetPos() - e0->p1.GetPos()).dot(e1->p0.GetPos() - e1->p1.GetPos()) < 0)
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
			Vertex* v0 = &f0->getVertices()[j];
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
					Vertex* v1 = &f1->getVertices()[l];
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

			newFaces.push_back(Face({ *(ptsFound[0]), *(ptsFound[1]), *(ptsFound[2]) }, Color(0, 0, 1)));
		}
	}

	//Ajout des nouvelles faces
	for (size_t i = 0; i < newFaces.size(); i++)
	{
		outputFaces.push_back(newFaces[i]);
	}

	return outputFaces;
}
