#include "Subdivision.h"

#define PI 3.141592

vector<Face> DooSabin(vector<Face> inputFaces)
{
	vector<Face> outputFaces;

	//Generatation des faces correspondantes aux existantes
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

			newPts.push_back(Vertex(newPos, Color(1, 0, 0)));
		}

		outputFaces.push_back(Face(newPts));
	}

	//TODO : Generation des nouvelles faces

	return outputFaces;
}
