#include "Coons.h"

Mesh generateCoon(std::vector<Curve> curves, float u, float v)
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
	//Mesh m(TopDown, top.getCurvePoints().size(), top.getCurvePoints().size() - 1);

	//2eme étape 
	std::vector<Vertex> LeftRight;
	generateRuledSurface(LeftRight, left, right, top.getCurvePoints().size());
	Mesh m(LeftRight, left.getCurvePoints().size(), left.getCurvePoints().size() - 1);

	std::vector<Vertex> ptsTop;
	std::vector<Vertex> ptsDown;

	float sizeTop = 0;
	float sizeDown = 0;
	for (size_t i = 0; i < top.getCurvePoints().size() - 1; i++)
	{
		sizeTop += (top.getCurvePoints()[i + 1].GetPos() - top.getCurvePoints()[i].GetPos()).magnitude();
		sizeDown += (down.getCurvePoints()[i + 1].GetPos() - down.getCurvePoints()[i].GetPos()).magnitude();
	}

	//top
	Vec3 T0T1 = top.getCurvePoints()[top.getCurvePoints().size() - 1].GetPos() - top.getCurvePoints()[0].GetPos();
	float ratioTop = sizeTop / T0T1.magnitude();
	Vertex p = top.getCurvePoints()[0];
	ptsTop.push_back(p);
	for (size_t i = 0; i < top.getCurvePoints().size() - 1; i++)
	{
		float distToNext = (top.getCurvePoints()[i].GetPos() - top.getCurvePoints()[i + 1].GetPos()).magnitude() * ratioTop;
		Vertex nextP = p + T0T1 * distToNext / T0T1.magnitude();
		ptsTop.push_back(nextP);
		p = nextP;
	}

	//Down
	Vec3 D0D1 = down.getCurvePoints()[down.getCurvePoints().size() - 1].GetPos() - down.getCurvePoints()[0].GetPos();
	float ratioDown = sizeDown / D0D1.magnitude();
	p = down.getCurvePoints()[0];
	ptsDown.push_back(p);
	for (size_t i = 0; i < down.getCurvePoints().size() - 1; i++)
	{
		float distToNext = (down.getCurvePoints()[i].GetPos() - down.getCurvePoints()[i + 1].GetPos()).magnitude() * ratioDown;
		Vertex nextP = p + D0D1 * distToNext / D0D1.magnitude();
		ptsDown.push_back(nextP);
		p = nextP;
	}

	std::vector<Vertex> quad;

	//TODO : Creer les courbes avec les points
	//Faire la fonction pour créer le plan en fonction des écartement
	//Créer le mesh
	//Faire la somme de tout

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