#include "Coons.h"

int etape = 3;

Mesh generateCoon(std::vector<Curve>& curves, float u, float v)
{
	Curve top = curves[0];
	Curve down;
	Curve left;
	Curve right;
	
	//rangement des courbes dans le bon sens
	for (size_t i = 1; i < curves.size(); i++)
	{
		if (top.getCurvePoints()[0] == curves[i].getCurvePoints()[curves[i].getCurvePoints().size() -1])
			left = curves[i];
		else if (top.getCurvePoints()[top.getCurvePoints().size()-1] == curves[i].getCurvePoints()[curves[i].getCurvePoints().size()-1])
			right = curves[i];
		else if (top.getCurvePoints()[0] != curves[i].getCurvePoints()[0] 
		 && top.getCurvePoints()[top.getCurvePoints().size() - 1] != curves[i].getCurvePoints()[curves[i].getCurvePoints().size() - 1])
			down = curves[i];
	}
	
	//1ere étape
	std::vector<Vertex> TopDown;
	generateRuledSurface(TopDown, down, top, top.getCurvePoints().size());
	//2eme étape 
	std::vector<Vertex> LeftRight;
	generateRuledSurface(LeftRight, left, right, top.getCurvePoints().size());

	//3eme étape
	std::vector<Vertex> ptsLeft;
	std::vector<Vertex> ptsRight;
	std::vector<Vertex> gridPts;

	GeneratePointsBetweenExtremities(ptsLeft, left.getCurvePoints(), left.getCurvePoints()[0], left.getCurvePoints()[left.getCurvePoints().size() - 1]);
	GeneratePointsBetweenExtremities(ptsRight, right.getCurvePoints(), right.getCurvePoints()[0], right.getCurvePoints()[right.getCurvePoints().size() - 1]);

	for (size_t i = 0; i < ptsLeft.size(); i++)
	{
		GeneratePointsBetweenExtremities(gridPts, ptsLeft, ptsLeft[i], ptsRight[i]);
	}


	//Faire la somme de tout
	std::vector<Vertex>coonsSurface;

	for (size_t i = 0; i < gridPts.size(); i++)
	{
		coonsSurface.push_back(Vertex(TopDown[i].GetPos() + LeftRight[i].GetPos() - gridPts[i].GetPos()));
	}

	Mesh m;

	//Etape 1
	if(etape == 0)
		m = Mesh(TopDown, top.getCurvePoints().size(), top.getCurvePoints().size() - 1);
	else if (etape == 1)
		m = Mesh(LeftRight, left.getCurvePoints().size(), left.getCurvePoints().size() - 1);
	else if(etape == 2)
		m = Mesh(gridPts, ptsLeft.size(), top.getCurvePoints().size() - 1);
	//Etape le boss final
	else if(etape == 3)
		m = Mesh(coonsSurface, ptsLeft.size(), top.getCurvePoints().size() - 1);
	else
		m = Mesh(coonsSurface, ptsLeft.size(), top.getCurvePoints().size() - 1);

	return m;

}

void generateRuledSurface(std::vector<Vertex>& ruledSurface, Curve c1, Curve c2, float resolution) //iteration : nbre de points dans left pour top
{

	for (size_t x = 0; x < c1.getCurvePoints().size(); x++)
	{
		//remplir avec la curve 1 
		ruledSurface.push_back(c1.getCurvePoints()[x]);

		//distance entre c1 et c2
		Vec3 C1C2 = c2.getCurvePoints()[x].GetPos() - c1.getCurvePoints()[x].GetPos();
		float factor = C1C2.magnitude() / ( resolution - 1 );

		for (size_t y = 1; y < resolution - 1; y++)
		{
			Vertex newV(c1.getCurvePoints()[x].GetPos() + C1C2 * factor * y);
			ruledSurface.push_back(newV);
		}

		//remplir avec la curve 2
		ruledSurface.push_back(c2.getCurvePoints()[x]);
	}
	
}

void GeneratePointsBetweenExtremities(std::vector<Vertex>& resultPoints, std::vector<Vertex> points, Vertex start, Vertex end)
{
	//Calcul de la distance totale entre les points
	float curveSize = 0;
	for (size_t i = 0; i < points.size() - 1; i++)
	{
		curveSize += (points[i + 1].GetPos() - points[i].GetPos()).magnitude();
	}

	Vec3 P0P1 = end.GetPos() - start.GetPos(); // Vecteur entre les 2 extrémité
	float sizeRatioDown = P0P1.magnitude() / curveSize; // ratio entre la taille de la courbe et celle entre les 2 extrémités

	Vertex currentPoint = start;
	resultPoints.push_back(start);

	for (size_t i = 0; i < points.size() - 1; i++)
	{
		float distToNext = (points[i].GetPos() - points[i + 1].GetPos()).magnitude() * sizeRatioDown; //Distance jusqu'a prochain point
		Vertex nextP = currentPoint + P0P1 * distToNext / P0P1.magnitude(); //Calcul de la position du prochain point

		resultPoints.push_back(nextP);
		currentPoint = nextP;
	}
}