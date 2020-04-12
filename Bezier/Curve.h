#pragma once
#include <vector>
#include "Vertex.h"

using namespace std;

class Curve
{
	vector<Vertex> controlPoints;
	vector<Vertex> curvePoints;
	float step;
	uint32_t VBOControl;
	uint32_t VBOCurve;
	Color color;

public:
	Curve();
	Curve(vector<Vertex> controlPoints, vector<Vertex> curvePoints, Color col);
	Curve(vector<Vertex> controlPoints, float step, Color color);

	void createBeziers(std::vector<Vertex>& curvePoints, std::vector<Vertex> controlPoints, float step, Color choosedColor);
	void updateCurve();

	void clearControlPoints() { controlPoints.clear(); }
	void clearCurvePoints() { curvePoints.clear(); }
	void setControlPoints(vector<Vertex> v);

	vector<Vertex> getControlPoints()const { return controlPoints; }
	vector<Vertex> getCurvePoints()const { return curvePoints; }
	uint32_t getVBOControl()const { return VBOControl; }
	uint32_t getVBOCurve()const { return VBOCurve; }
};

