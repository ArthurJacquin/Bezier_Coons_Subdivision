#include "Coons.h"

Mesh generateCoon(std::vector<Curve> curves)
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
	//generateRuledSurface(TopDown, down, top, top.getCurvePoints().size());
	//Mesh m(TopDown, top.getCurvePoints().size(), top.getCurvePoints().size() - 1);

	//2eme étape 
	std::vector<Vertex> LeftRight;
	generateRuledSurface(LeftRight, left, right, top.getCurvePoints().size());
	Mesh m(LeftRight, left.getCurvePoints().size(), left.getCurvePoints().size() - 1);

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
